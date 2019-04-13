#include "H2Tool_Commands.h"
#include "Common\H2EKCommon.h"
#include "Common\TagInterface.h"
#include "Tags\ScenarioTag.h"
#include "Tags\Globals.h"
#include "Tags\HudGlobals.h"
#include "H2ToolLibrary.inl"

/*
	Fixes the haloscript compiler not generating the correct data for some types.
*/

static hs_script_node *hs_get_script_node(unsigned __int16 index)
{
	void *script_nodes = *reinterpret_cast<DWORD**>(0xBCBF4C);
	return reinterpret_cast<hs_script_node*>(get_object_at_data_array_index(script_nodes, index));
}

// get the string from a syntax node
static const char *hs_get_string_data(hs_script_node *syntax_node)
{
	const char *hs_string_data = *reinterpret_cast<const char **>(0x00CDB198);
	return &hs_string_data[syntax_node->string_value_offset];
}

char hs_error[0x1024];

// helper function for reporting an error parsing a syntax element
static void hs_converter_error(hs_script_node *script_node, const std::string &error)
{
	const char **hs_error_string_ptr = reinterpret_cast<const char**>(0x00CDB1AC);
	DWORD *hs_error_offset_ptr = reinterpret_cast<DWORD*>(0x00CDB1B0);

	strncpy(hs_error, error.c_str(), sizeof(hs_error));

	*hs_error_string_ptr = hs_error;
	*hs_error_offset_ptr = script_node->string_value_offset;
	script_node->value = NONE;
}

// returns scenario pointer
static scnr_tag *get_global_scenario()
{
	return *reinterpret_cast<scnr_tag **>(0x00AA00E4);
}

/* 
	Sets the syntax node's value to the index of the element which contains the string id
*/
static void hs_convert_string_id_to_tagblock_index(tag_block_ref *tag_block, int element_size, int block_offset, int script_node_index)
{
	auto script_node = hs_get_script_node(script_node_index);
	const char *value_string = hs_get_string_data(script_node);

	script_node->value = FIND_TAG_BLOCK_STRING_ID(tag_block, element_size, block_offset, GET_STRING_ID(value_string));

	if (script_node->value == NONE) {
		std::string error = "this is not a valid '" + hs_type_string[static_cast<hs_type>(script_node->value_type)] + "' name, check tags";
		hs_converter_error(script_node, error);
	}
}

/*
	Sets the syntax node's value to the index of the element which contains the string
*/
static void hs_convert_string_to_tagblock_offset(tag_block_ref *tag_block, int element_size, int block_offset, int script_node_index)
{
	auto script_node = hs_get_script_node(script_node_index);
	const char *value_string = hs_get_string_data(script_node);

	script_node->value = FIND_TAG_BLOCK_STRING(tag_block, element_size, block_offset, value_string);

	if (script_node->value == NONE) {
		std::string error = "this is not a valid '" + get_hs_type_string(script_node->value_type) + "' name, check tags";
		hs_converter_error(script_node, error);
	}
}

static char __cdecl hs_convert_conversation(unsigned __int16 script_node_index)
{
	scnr_tag *scenario = get_global_scenario();
	hs_convert_string_to_tagblock_offset(&scenario->aIConversations, 128, 0, script_node_index);
	return 1;
}

static char __cdecl hs_convert_internal_id_passthrough(unsigned __int16 index)
{
	hs_script_node *script_node = hs_get_script_node(index);
	const char *input_string = hs_get_string_data(script_node);
	if (!_stricmp(input_string, "NONE")) {
		script_node->value = NONE;
		return 1;
	}
	try {
		script_node->value = std::stoi(input_string, nullptr, 0);
		return 1;
	}
	catch (invalid_argument) {
		hs_converter_error(script_node, "invalid " + get_hs_type_string(script_node->value_type) + " ID");
		return 0;
	}
	catch (out_of_range)
	{
		hs_converter_error(script_node, get_hs_type_string(script_node->value_type) + " ID out of range");
		return 0;
	}
}

static char __cdecl hs_convert_ai_behaviour(unsigned __int16 script_node_index)
{
	hs_script_node *script_node = hs_get_script_node(script_node_index);
	const std::string input = hs_get_string_data(script_node);
	ai_behaviour out = string_to_ai_behaviour(input);
	if (out != ai_behaviour::invalid) {
		script_node->value = static_cast<DWORD>(out);
		return 1;
	}
	else {
		hs_converter_error(script_node, "Invalid AI behaviour");
		return 0;
	}
}

static char __cdecl hs_convert_ai_orders(unsigned __int16 script_node_index)
{
	scnr_tag *scenario = get_global_scenario();
	hs_convert_string_to_tagblock_offset(&scenario->orders, 144, 0, script_node_index);
	return 1;
}

enum ai_id_type
{
	squad,
	squad_group,
	unknown,
	starting_location,

	none = NONE
};

static char __cdecl hs_convert_ai(unsigned __int16 script_node_index)
{
	scnr_tag *scenario = get_global_scenario();
	hs_script_node *script_node = hs_get_script_node(script_node_index);
	std::string input_string = hs_get_string_data(script_node);

	ai_id_type ai_type = ai_id_type::none;

	DWORD main_index = NONE;
	DWORD secondary_index = 0;

	if (input_string.find('/') != string::npos) {
		ai_type = ai_id_type::starting_location;
		std::string squad_name = input_string.substr(0, input_string.find('/'));
		std::string squad_pos = input_string.substr(input_string.find('/') + 1);

		secondary_index = scenario->squads.find_string_element(0, squad_name);
		if (secondary_index != NONE) {
			// can't get the squad block struct working so this is a workaround for now
			main_index = strtol(squad_pos.c_str(), nullptr, 0);
		} else {
			hs_converter_error(script_node, "No such squad.");
			return 0;
		}
	} else {
		ai_type = ai_id_type::squad;
		// attempt to find a squad with that name first
		main_index = scenario->squads.find_string_element(0, input_string);
		if (main_index == NONE) {
			// if no sqaud with that name exists try the squad groups
			ai_type = ai_id_type::squad_group;
			main_index = scenario->squadGroups.find_string_element(offsetof(squad_groups_block, name), input_string);
		}
	}
	if (main_index != NONE) {
		DWORD id = (ai_type << 30) | (secondary_index << 16) | main_index;
		script_node->value = id;
		return 1;
	}
	else {
		// fallback to passthrough for backwards compatibility and AI squad starting locations
		return hs_convert_internal_id_passthrough(script_node_index);
	}
}

static char __cdecl hs_convert_point_ref(unsigned __int16 script_node_index)
{
	scnr_tag *scenario = get_global_scenario();
	hs_script_node *script_node = hs_get_script_node(script_node_index);
	std::string input_string = hs_get_string_data(script_node);
	auto scripting_data = scenario->scriptingData[0];

	if (!scripting_data) {
		hs_converter_error(script_node, "Script data missing from scenario.");
		return false;
	}

	if (input_string.find('/') != string::npos) {
		std::string point_set = input_string.substr(0, input_string.find('/'));
		std::string point = input_string.substr(input_string.find('/') + 1);

		size_t point_set_index = scripting_data->pointSets.find_string_element(offsetof(cs_point_set_block, name), point_set);
		if (point_set_index != NONE) {
			cs_point_set_block *points_set = scripting_data->pointSets[point_set_index];
			size_t point_index = points_set->points.find_string_element(offsetof(cs_point_block, name), point);

			if (point_index != NONE) {
				script_node->value = (point_set_index << 16 | point_index);
				return 1;
			} else {
				hs_converter_error(script_node, "No such point.");
				return false;
			}
		}
		else {
			hs_converter_error(script_node, "No such point set.");
			return false;
		}
	}
	else {
		hs_converter_error(script_node, "Invalid format.");
		return false;
	}
}

static char __cdecl hs_convert_navpoint(unsigned __int16 script_node_index)
{
	hs_script_node *script_node = hs_get_script_node(script_node_index);
	auto globals = tags::load_tag('matg', "globals/globals", 4);
	if (!LOG_CHECK(globals.is_valid()))
	{
		hs_converter_error(script_node, "WTF globals are missing...");
		return false;
	}
	auto globals_data = tags::get_tag<globals_block>('matg', globals);
	assert(globals_data);

	auto interface_info = globals_data->interfaceTags[0];
	if (!interface_info || !interface_info->hudGlobals.tag_index.is_valid())
	{
		hs_converter_error(script_node, "\"Interface Tags\" is missing from globals or doesn't link to hub globals.");
		return false;
	}

	auto hub_globals = tags::get_tag<hud_globals_block>('hudg', interface_info->hudGlobals.tag_index);
	assert(hub_globals);

	auto &way_points = hub_globals->newGlobals.waypoints;
	way_points.find_string_element()
}

#define set_hs_converter(type, func) \
	hs_convert_lookup_table[static_cast<int>(type)] = func;

void H2ToolPatches::fix_hs_converters()
{
	void **hs_convert_lookup_table = reinterpret_cast<void**>(0x009F0C88);
	set_hs_converter(hs_type::ai_behavior, hs_convert_ai_behaviour);
	set_hs_converter(hs_type::conversation, hs_convert_conversation);
	set_hs_converter(hs_type::ai_orders, hs_convert_ai_orders);
	set_hs_converter(hs_type::ai, hs_convert_ai);
	set_hs_converter(hs_type::point_reference, hs_convert_point_ref);

	// hacky workaround, lets the user directly input the ID it's meant to generate.
	hs_type passthrough_types[] = {
		hs_type::style,           hs_type::hud_message,
		hs_type::navpoint,        hs_type::ai_command_list
	};

	for (auto i : passthrough_types)
		set_hs_converter(i, hs_convert_internal_id_passthrough);
}

#undef set_hs_converter
