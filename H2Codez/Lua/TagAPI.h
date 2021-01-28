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

		sol::usertype<FieldDefinition> field_def_type = lua.new_usertype<FieldDefinition>("LuaTagAPI::FieldDefinition");
		field_def_type["name"] = sol::readonly_property(&FieldDefinition::get_name);
	};

	struct TagStructView {

	};

	struct Tag {
		Tag(datum tag) :
			tag_handle(tag)
		{
			if (is_debug_build())
				ASSERT_CHECK(tag != NONE);
		}

		tags::s_scoped_handle tag_handle;

		bool save() const {
			return tags::save_tag(tag_handle);
		}

		bool set_name(const std::string& new_name) const {
			return tags::rename_tag(tag_handle, new_name);
		}

		std::string get_name() const {
			return tags::get_name(tag_handle);
		}
	};

	struct FieldDefinition {
		tag_field* definition;
		size_t offset;

		std::string get_name() const {
			verify();
			return definition->name.get_string();
		}

	private:
		void verify() const {
			if (is_debug_build())
				ASSERT_CHECK(definition != nullptr);
		}
	};

	struct Field {
		FieldDefinition definition;
		union {
			char *string;
			string_id *string_id;
			int8_t *byte_int;
			int16_t *short_int;
			int32_t *long_int;
			angle *angle;
			blam_tag *tag;
			size_t untyped;
			void *untyped_pointer;
		} data;
		CHECK_STRUCT_SIZE(data, sizeof(void*));

		void invalidate() {
			data.untyped_pointer = nullptr;
		}

		void update_data_pointer(void* new_struct_data) {
			if (is_debug_build())
				ASSERT_CHECK(new_struct_data);
			data.untyped = reinterpret_cast<size_t>(new_struct_data) + definition.offset;
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
		const datum tag = tags::load_tag(string_to_tag_type(type), name, tags::for_editor);
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