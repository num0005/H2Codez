#pragma once
#include "API.h"
#include "Common/TagInterface.h"
#include "Common/tag_group_names.h"

class LuaTagAPI : public LuaAPI {
public:
	LuaTagAPI(sol::state& _lua) : LuaAPI(_lua)
	{
		lua.set("tags", this);
		sol::usertype<LuaTagAPI> tag_api_type = lua.new_usertype<LuaTagAPI>("LuaTagAPI");
		tag_api_type["new"] = &LuaTagAPI::new_tag;
		tag_api_type["load"] = &LuaTagAPI::load_tag;

		sol::usertype<Tag> tag_type = lua.new_usertype<Tag>("LuaTagAPI::Tag");
		tag_type["save"] = &Tag::save;
		tag_type["name"] = sol::property(&Tag::get_name, &Tag::set_name);
		tag_type["read_only"] = sol::readonly(&Tag::is_read_only);
		tag_type["view"] = sol::readonly_property(&Tag::get_view);

		sol::usertype<FieldDefinition> field_def_type = lua.new_usertype<FieldDefinition>("LuaTagAPI::FieldDefinition");
		field_def_type["name"] = sol::readonly_property(&FieldDefinition::get_name);
		field_def_type["group_tag"] = sol::readonly_property(&FieldDefinition::get_tag);
		field_def_type["type"] = sol::readonly_property(&FieldDefinition::get_type_string);
		field_def_type["offset"] = sol::readonly(&FieldDefinition::offset);
		field_def_type["size"] = sol::readonly(&FieldDefinition::size);

		sol::usertype<Field> field_type = lua.new_usertype<Field>("LuaTagAPI::Field");
		field_type["definition"] = sol::readonly(&Field::definition);
		field_type["data"] = sol::property(&Field::getter, &Field::setter);
		field_type["elements"] = sol::readonly_property(&Field::get_block_elements);
		
		sol::usertype<TagStructView> tag_struct_view = lua.new_usertype<TagStructView>("LuaTagAPI::TagStructView");
		tag_struct_view["contents"] = sol::readonly(&TagStructView::contents);

		lua.new_usertype<tag_reference>("tag_reference",
			sol::constructors<tag_reference()>(),
			sol::meta_function::to_string, [](tag_reference& ref) -> std::string {
				return stringify("type", ref.tag_type.as_string(), "name", ref.tag_name);
			}, 
			"type", &tag_reference::tag_type,
			"name", sol::readonly(&tag_reference::tag_name)
		);

		lua.new_usertype<byte_ref>("data",
			sol::no_constructor,
			sol::meta_function::to_string, [](byte_ref& ref) -> std::string {
				return stringify("size", ref.size);
			}, 
			"size", sol::property(&byte_ref::size, &byte_ref::resize),
			"data", sol::property(
				[](byte_ref &ref) -> std::string {
					const auto data = ref.c_str();
					const auto size = ref.size;
					if (!data || !size)
						return "";
					return std::string(data, size);
				},
				[](byte_ref& ref, const std::string& value) {
					const auto data = value.c_str();
					const auto size = value.size();
					ASSERT_CHECK(size < MAXINT);
					if (size != ref.size)
						ref.resize(static_cast<int>(size));
					memcpy(ref[0], data, size);
				}
			)
		);
	};

	struct FieldDefinition {
		const tag_field* definition;
		size_t offset;
		size_t size;

		std::string get_name() const {
			verify();
			return definition->name.get_string();
		}

		std::string get_tag() const {
			verify();
			return definition->group_tag.as_string();
		}

		tag_field::field_type get_type() const {
			verify();
			return definition->type;
		}

		const char* get_type_string() const {
			return tag_field_type_names[get_type()];
		}

	private:
		void verify() const {
			if (is_debug_build())
				ASSERT_CHECK(definition != nullptr);
		}
	};

	struct TagStructView;

	struct Field {
		Field(const tag_field *def, size_t field_offset, size_t field_size, void *field_data):
			definition({ def, field_offset, field_size }),
			data({ field_data })
		{
		}

		FieldDefinition definition;
		union {
			void* untyped_pointer;
			size_t untyped;
			char *string;
			string_id *string_id;
			int8_t *byte_int;
			int16_t *short_int;
			int32_t *long_int;
			angle *angle;
			blam_tag *tag;
			float* real;
			point2d* point_2d;
			rect2d* rect;
			real_point2d* real_point_2d;
			real_point3d* real_point_3d;
			real_vector2d* real_vector_2d;
			real_vector3d* real_vector_3d;
			real_quaternion* quaternion;
			real_euler_angles2d* angle_2d;
			real_euler_angles3d* angle_3d;
			real_plane2d* plane_2d;
			real_plane3d* plane_3d;
			colour_rgb* real_colour_rbg;
			colour_rgba* real_colour_arbg;
			tag_reference* reference;
			byte_ref* data;
			tag_block<void> *block;
		} data;
		CHECK_STRUCT_SIZE(data, sizeof(void*));

		std::shared_ptr<std::vector<TagStructView>> block;

#define WRAP_TYPE(data) sol::make_object(lua, ##data)
#define NOT_IMPL sol::make_object(lua, "DEBUG: NOT IMPL!")

		sol::object getter(sol::this_state s) {
			if (!data.untyped_pointer)
				throw std::runtime_error("Attempt to use deleted field!");
			sol::state_view lua(s);

			switch (definition.get_type()) {
				case tag_field::string:
				case tag_field::long_string:
					return WRAP_TYPE(data.string);
				case tag_field::string_id:
				case tag_field::old_string_id:
					return WRAP_TYPE(*data.string_id);
				case tag_field::char_integer:
				case tag_field::char_block_index1:
				case tag_field::char_block_index2:
				case tag_field::char_enum: // todo remove
				case tag_field::byte_flags: // todo remove
					return WRAP_TYPE(*data.byte_int);
				case tag_field::short_integer:
				case tag_field::short_block_index1:
				case tag_field::short_block_index2:
				case tag_field::generic_enum: // todo remove
				case tag_field::word_flags: // todo remove
					return WRAP_TYPE(*data.short_int);
				case tag_field::long_integer:
				case tag_field::long_block_index1:
				case tag_field::long_block_index2:
				case tag_field::long_enum: // todo remove
				case tag_field::long_flags: // todo remove
					return WRAP_TYPE(*data.long_int);
				case tag_field::angle:
					return WRAP_TYPE(data.angle);;
				case tag_field::tag:
					return WRAP_TYPE(data.tag);
				//case tag_field::char_enum:
				//case tag_field::generic_enum:
				//case tag_field::long_enum:
				//case tag_field::long_flags:
				//case tag_field::word_flags:
				//case tag_field::byte_flags:
					//return NOT_IMPL;
				case tag_field::point_2d:
					return WRAP_TYPE(data.point_2d);
				case tag_field::rectangle_2d:
					return WRAP_TYPE(data.rect);
				case tag_field::rgb_color:
				case tag_field::argb_color:
					return WRAP_TYPE(*data.long_int);
				case tag_field::real:
				case tag_field::real_fraction:
					return WRAP_TYPE(*data.real);
				case tag_field::real_point_2d:
					return WRAP_TYPE(data.real_point_2d);
					//return as_table(lua, std::map<std::string, float>{ { "x", data.real_point_2d->x }, { "y", data.real_point_2d->y } });
				case tag_field::real_point_3d:
					return WRAP_TYPE(data.real_point_3d);
					//return as_table(lua, std::map<std::string, float>{ { "x", data.real_point_3d->x }, { "y", data.real_point_3d->y }, { "z", data.real_point_3d->z } });
				case tag_field::real_vector_2d:
					return WRAP_TYPE(data.real_vector_2d);
					//return as_table(lua, std::map<std::string, float>{ { "i", data.real_point_2d->x }, { "j", data.real_point_2d->y } });
				case tag_field::real_vector_3d:
					return WRAP_TYPE(data.real_vector_3d);
					//return as_table(lua, std::map<std::string, float>{ { "i", data.real_point_3d->x }, { "j", data.real_point_3d->y }, { "k", data.real_point_3d->z } });
				case tag_field::real_quaternion:
					return WRAP_TYPE(data.quaternion);
					//return as_table(lua, std::map<std::string, float>{ { "i", data.quaternion->i }, { "j", data.quaternion->j }, { "k", data.quaternion->k }, { "w", data.quaternion->w } });
				case tag_field::real_euler_angles_2d:
					return WRAP_TYPE(data.angle_2d);
				case tag_field::real_euler_angles_3d:
					return WRAP_TYPE(data.angle_3d);
				case tag_field::real_plane_2d:
					return WRAP_TYPE(data.plane_2d);
				case tag_field::real_plane_3d:
					return WRAP_TYPE(data.plane_3d);
				case tag_field::real_rgb_color:
					return WRAP_TYPE(data.real_colour_rbg);
					/*
					return as_table(lua, std::map<std::string, float>{
						{ "red", data.real_colour_rbg->red },
						{ "green",  data.real_colour_rbg->green },
						{ "blue",  data.real_colour_rbg->blue } 
					});*/
				case tag_field::real_argb_color:
					return WRAP_TYPE(data.real_colour_arbg);
					/*return as_table(lua, std::map<std::string, float>{
						{ "red", data.real_colour_arbg->red },
						{ "green",  data.real_colour_arbg->green },
						{ "blue",  data.real_colour_arbg->blue },
						{ "alpha",  data.real_colour_arbg->alpha }
					});*/
				case tag_field::real_hsv_color:
				case tag_field::real_ahsv_color:
				case tag_field::short_bounds:
				case tag_field::angle_bounds:
				case tag_field::real_bounds:
				case tag_field::real_fraction_bounds:
					return NOT_IMPL;
				case tag_field::tag_reference:
					return WRAP_TYPE(data.reference);
					//return as_table(lua, std::map<std::string, std::string>{
					//	{ "type", H2CommonPatches::tag_group_names.at(data.reference->tag_type.as_int()) },
					//	{ "path",  data.reference->tag_name }
					//});
				case tag_field::block:
					return sol::make_object(lua, get_block_elements());
				case tag_field::long_block_flags:
				case tag_field::word_block_flags:
				case tag_field::byte_block_flags:
					return NOT_IMPL;
				case tag_field::data:
					return WRAP_TYPE(data.data);
				case tag_field::vertex_buffer:
				case tag_field::array_start:
					return NOT_IMPL;
				case tag_field::array_end:
					return WRAP_TYPE(sol::lua_nil);
				case tag_field::pad:
				case tag_field::skip:
				case tag_field::explanation:
				case tag_field::custom:
					return NOT_IMPL;
				default:
					throw std::exception("Unexpected field type!");

			}
		}
#undef WRAP_TYPE

		void setter(sol::this_state s, sol::object value) {
			if (!data.untyped_pointer)
				throw std::runtime_error("Attempt to use deleted field!");

			sol::state_view lua(s);

#define UNWRAP_TYPE(field) \
	*data.##field = value.as<decltype((*data.##field))>(); \
	break
			switch (definition.get_type()) {
				case tag_field::string:
				case tag_field::long_string:
				{
					const auto new_data = value.as<std::string>();
					const size_t max_length = definition.get_type() == tag_field::long_string ? 0x100 : 0x20;
					if (new_data.size() >= max_length)
						throw std::runtime_error("String is too long! (" 
							+ std::to_string(new_data.size()) + " > " + std::to_string(max_length) + ")");
					strncpy_s(data.string, max_length, new_data.c_str(), new_data.size());
					break;
				}
				case tag_field::string_id:
				case tag_field::old_string_id:
					UNWRAP_TYPE(string_id);
				case tag_field::char_integer:
				case tag_field::char_block_index1:
				case tag_field::char_block_index2:
				case tag_field::char_enum: // todo remove
				case tag_field::byte_flags: // todo remove
					UNWRAP_TYPE(byte_int);
				case tag_field::short_integer:
				case tag_field::short_block_index1:
				case tag_field::short_block_index2:
				case tag_field::generic_enum: // todo remove
				case tag_field::word_flags: // todo remove
					UNWRAP_TYPE(short_int);
				case tag_field::long_integer:
				case tag_field::long_block_index1:
				case tag_field::long_block_index2:
				case tag_field::long_enum: // todo remove
				case tag_field::long_flags: // todo remove
					UNWRAP_TYPE(long_int);
				case tag_field::rgb_color:
				case tag_field::argb_color:
					UNWRAP_TYPE(long_int);
				case tag_field::real:
				case tag_field::real_fraction:
					UNWRAP_TYPE(real);
				default:
					throw std::runtime_error("Writing to this field type is no implemented!");
			}
#undef UNWRAP_TYPE
		}

		void invalidate() noexcept {
			data.untyped_pointer = nullptr;
			if (block) {
				for (size_t i = 0; i < block->size(); i++)
					block->at(i).invalidate();
			}
		}

		void update_data_pointer(void* new_struct_data) {
			if (is_debug_build())
				ASSERT_CHECK(new_struct_data);
			data.untyped = reinterpret_cast<size_t>(new_struct_data) + definition.offset;

			if (block) {
				for (size_t i = 0; i < block->size(); i++)
					block->at(i).invalidate();
			}
		}

		std::vector<TagStructView> *get_block_elements() {
			if (definition.get_type() != tag_field::block)
				throw std::runtime_error("Not a tag block!");

			if (!block) {
				auto elements = std::make_unique<std::vector<TagStructView>>();
				for (auto i = 0; i < data.block->size; i++)
					elements->push_back(TagStructView(
						data.block->defination,
						data.block->get_element(static_cast<size_t>(i))
					));
				block = std::move(elements);
			}
			return block.get();
		}

	private:
		template<typename T>
		sol::object as_table(sol::state_view &lua, const std::map<std::string, T> &data) {
			return sol::make_object(lua, sol::as_table(data));
		}
	};

	typedef std::vector<Field> tag_struct_view;

	struct TagStructView {
		TagStructView(const tag_field* fields, void* data) {
			init_view(fields, data);
		}

		TagStructView(const tag_block_defintions *definition, void *data) {
			init_view(definition->latest->fields, data);
		}

		~TagStructView() {
			//LOG_FUNC("Deleting struct view %p", this);
			invalidate();
		}

		void invalidate() {
			for (Field& field : contents)
				field.invalidate();
		}

		void update_data_pointer(void* new_struct_data) {
			if (is_debug_build())
				ASSERT_CHECK(new_struct_data);
			for (Field& field : contents)
				field.update_data_pointer(new_struct_data);
		}

		void init_view(const tag_field* fields, void* data) {
			tags::s_tag_field_iterator iterator(fields, data);
			iterator.enable_all_fields();
			iterator.disable_field_type(tag_field::useless_pad);
			iterator.disable_field_type(tag_field::struct_);

			while (iterator.next()) {
				contents.push_back(
					Field(iterator.current_field,
						iterator.current_field_offset,
						iterator.current_field_size,
						iterator.current_field_data
					)
				);
			}
		}
		tag_struct_view contents;
	};

	struct Tag {
		Tag(datum tag) :
			tag_handle(tag)
		{
			if (is_debug_build())
				ASSERT_CHECK(tag != NONE);
		}

		tags::s_scoped_handle tag_handle;
		std::unique_ptr<TagStructView> view;

		bool save() const {
			return tags::save_tag(tag_handle);
		}

		bool set_name(const std::string& new_name) const {
			return tags::rename_tag(tag_handle, new_name);
		}

		bool save_as(const std::string& new_name) const {
			std::string old_name = get_name();
			ASSERT_CHECK(set_name(new_name));
			bool success = save();
			ASSERT_CHECK(set_name(old_name));
			return success;
		}

		std::string get_name() const {
			return tags::get_name(tag_handle);
		}

		bool is_read_only() const {
			return tags::is_read_only(tag_handle);
		}

		tag_struct_view *get_view() {
			if (!view) {
				LOG_FUNC("Creating struct view");
				tag_block<> root = *tags::get_root_block(tag_handle);
				if (is_debug_build())
					ASSERT_CHECK(root.size == 1);
				view.reset(new TagStructView(root.defination, root[0]));
			}
			return &view.get()->contents;
		}
	};

	Tag new_tag(const std::string& type, const std::string& name) {
		LOG_FUNC("type: %s, name: %s", type.c_str(), name.c_str());
		const datum tag = tags::new_tag(string_to_tag_type(type), name);
		if (tag == NONE)
			throw std::runtime_error("Failed to create tag!");
		return Tag{ tag };
	}

	Tag load_tag(const std::string& type, const std::string& name) {
		LOG_FUNC("type: %s, name: %s", type.c_str(), name.c_str());
		const datum tag = tags::load_tag(string_to_tag_type(type), name, tags::for_editor | tags::skip_child_tag_load);
		if (tag == NONE)
			throw std::runtime_error("Failed to create tag!");
		return Tag{ tag };
	}

private:
	inline int string_to_tag_type(const std::string &type) {
		const int tag_type = H2CommonPatches::string_to_tag_group(type);
		if (tag_type == NONE)
			throw std::runtime_error("No such tag type!");
		return tag_type;
	}
};