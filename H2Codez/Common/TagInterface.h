#pragma once
#include "BasicTagTypes.h"
#include "common/data/data_array.h"
#include "TagDefinitions.h"
#include "FiloInterface.h"

namespace tags
{
	inline s_data_array *get_tag_instances()
	{
		return *reinterpret_cast<s_data_array**>(SwitchAddessByMode(0xA801A0, 0xA75488, 0x9B7E20));
	}

	struct s_tag_instance
	{
		__int16 datum_header;
		__int16 flags;
		const char *name;
		int group_tag;
		int parent_group;
		int grandparent_group;
		char field_14;
		char read_only;
		char field_16;
		char field_17;
		char loading_finished__maybe;
		char field_19;
		char sub_4AEC40;
		char field_1B;
		int salt;
		tag_block_ref data;
		int next__maybe;
	};
	CHECK_STRUCT_SIZE(s_tag_instance, 0x30);


	struct s_tag_ilterator
	{
		void *next_tag_instance;
		s_data_array::s_ilterator ilterator;
		blam_tag tag_group;
		s_tag_ilterator(blam_tag tag = blam_tag::null()) :
			ilterator(get_tag_instances()),
			tag_group(tag)
		{
		};

		datum next()
		{
			typedef signed int __cdecl tag_next(s_tag_ilterator *a1);
			auto tag_next_impl = reinterpret_cast<tag_next*>(SwitchAddessByMode(0x52DD30, 0x4AF700, 0x4836D0));
			return tag_next_impl(this);
		}
	};
	CHECK_STRUCT_SIZE(s_tag_ilterator, 0x18);

	struct s_tag_field_ilterator
	{
		s_tag_field_ilterator(tag_field* fields, void* tag_data, int flag = 0) :
			field_defintions(fields),
			data(tag_data),
			flags(flag)
		{};

		void disable_all_fields() {
			memset(field_type_flags, 0, sizeof(field_type_flags));
		}

		void enable_all_fields() {
			memset(field_type_flags, -1, sizeof(field_type_flags));
		}

		void enable_field_type(tag_field::field_type type) {
			field_type_flags[type >> 5] |= (1 << (type & 31));
		}

		void disable_field_type(tag_field::field_type type) {
			field_type_flags[type >> 5] &= ~(1 << (type & 31));
		}

		tag_field* field_defintions = nullptr;
		void* data = nullptr;
		int flags = 0;
		int field_type_flags[2] = { };
		int current_index = 0;
		int current_field_size;
		int next_field_offset = 0;
		int field_offset_runtime = 0;
		char is_done = 0;
		char pad = 0;
		__int16 stack_index = 0;
		int stack[32] = {};
		tag_field* current_field = nullptr;
		void *current_field_data = nullptr;
		int current_field_offset = 0;
		int found_field = 0;

		bool next() {
			typedef char __cdecl next(s_tag_field_ilterator* iter);
			auto next_impl = reinterpret_cast<next*>(SwitchAddessByMode(0x52DAD0, 0u, 0u));
			CHECK_FUNCTION_SUPPORT(next_impl);
			return next_impl(this);
		}
	};
	CHECK_STRUCT_SIZE(s_tag_field_ilterator, 0xB8);


	/* Get tag_def for tag_group */
	tag_def *get_group_definition(int tag_group);

	/* Get tag group of tag */
	int get_group_tag(datum tag);

	/* Get tag_def for tag_group */
	inline tag_def *get_group_definition(datum tag)
	{
		return get_group_definition(get_group_tag(tag));
	}

	/* Get main tag block from tag */
	tag_block_ref *get_root_block(datum tag);

	/* Get tag name */
	std::string get_name(datum tag);

	/* Check if a tag of group exists at path */
	bool tag_exists(int group, std::string path);

	/* Is the tag file read-only */
	bool is_read_only(datum tag);

	void set_reference(tag_reference *reference, int group_tag, cstring name);
	void copy_reference(tag_reference *src, tag_reference *dest);

	/* Return the data for a tag */
	template<typename T = void>
	inline T *get_tag(blam_tag tag_group, datum index)
	{
		return reinterpret_cast<T*>(get_tag<void>(tag_group, index));
	}

	template<>
	void *get_tag<void>(blam_tag tag_group, datum index);

	/* Unload a tag */
	void unload_tag(datum index);

	/* Checks if tag is currently loaded */
	bool is_tag_loaded(blam_tag tag_group, const std::string name);

	/* Create new tag with group and name */
	datum new_tag(int group, const std::string name);

	enum loading_flags : int
	{
		for_editor                = 1 << 0,
		no_log                    = 1 << 1,
		skip_child_tag_load       = 1 << 2,
		skip_existing             = 1 << 3,
		skip_block_postprocess    = 1 << 4, // not sure
		byte_swap                 = 1 << 5, // not sure
		skip_tag_postprocess      = 1 << 6,
		dont_call_handle_invalid  = 1 << 7, // only works with loading_flags::for_editor

		no_post_processing = loading_flags::for_editor |	
			loading_flags::skip_child_tag_load | 
			loading_flags::skip_tag_postprocess | 
			loading_flags::skip_block_postprocess,
	};

	/* Loads a tag from file system */
	datum load_tag(blam_tag group, const std::string &name, loading_flags flags);

	/* Loads a tag from file system */
	inline datum load_tag(blam_tag group, const std::string &name, int flags)
	{
		return load_tag(group, name, static_cast<loading_flags>(flags));
	}

	/* Reloads tag data from file system if possible */
	size_t reload_tag(blam_tag group, const std::string name);

	/* Change the path the tag will be saved to */
	bool rename_tag(datum index, const std::string name);

	/* Save tag to disk */
	bool save_tag(datum index);

	/* Add a new element, returns index */
	template<typename T = void>
	unsigned int add_block_element(tag_block<T> *block)
	{
		return add_block_element(block->get_ref());
	}
	unsigned int add_block_element(tag_block_ref *block);

	/* Deletes element at index */
	template<typename T = void>
	bool delete_block_element(tag_block<T> *block, size_t index)
	{
		return delete_block_element(block->get_ref(), index);
	}
	bool delete_block_element(tag_block_ref *block, size_t index);

	/* Delete all tag block elements */
	template<typename T = void>
	bool block_delete_all(tag_block<T> *block)
	{
		return block_delete_all(block->get_ref());
	}
	bool block_delete_all(tag_block_ref *block);

	/* copy tag block elements */
	template<typename T = void>
	bool copy_block(tag_block<T> *source_block, tag_block<T> *dest_block)
	{
		return copy_block(source_block->get_ref(), dest_block->get_ref());
	}
	bool copy_block(tag_block_ref *source_block, tag_block_ref *dest_block);

	/* Change number of tag block elements */
	template<typename T = void>
	bool resize_block(tag_block<T> *block, size_t size)
	{
		return resize_block(block->get_ref(), size);
	}
	bool resize_block(tag_block_ref *block, size_t size);

	/* Get file_reference for tag */
	bool get_tag_filo(file_reference *file_ref, int tag_group, LPCSTR tag_path);
	inline bool get_tag_filo(file_reference *file_ref, datum tag)
	{
		std::string name = get_name(tag);
		int tag_group = get_group_tag(tag);
		return get_tag_filo(file_ref, tag_group, name.c_str());
	}

	class s_scoped_handle : public datum
	{
	public:
		s_scoped_handle() = default;
		s_scoped_handle(s_scoped_handle&& source):
			datum(source.as_long())
		{
			source.index = NONE;
			source.salt = NONE;
		}
		s_scoped_handle(const s_scoped_handle&) = delete;
		s_scoped_handle& operator=(const s_scoped_handle&) = delete;

		s_scoped_handle(const datum& tag) :
			datum(tag)
		{
		}

		s_scoped_handle(size_t info) :
			datum(info)
		{
		}

		s_scoped_handle& operator=(const datum& tag)
		{
			unload();
			index = tag.index;
			salt = tag.salt;
			return *this;
		}

		s_scoped_handle& operator=(const size_t& tag)
		{
			return operator=(datum(tag));
		}

		datum get_tag_datum() const
		{
			return as_long();
		}

		~s_scoped_handle()
		{
			unload();
		}

		void clear() {
			unload();
			datum::clear();
		}

	private:
		void unload()
		{
			if (is_valid())
				tags::unload_tag(*this);
		}
	};
};
