#include "LuaCommand.h"
#include "Lua/API.h"

static void _cdecl lua_proc(const wchar_t* argv[])
{
	auto lua_path = utf16_to_utf8(argv[0]);
	LuaInterface l_interface;
	auto error = l_interface.execute_script(lua_path);
	if (error.has_value())
		cout << "Lua Error: " << error.value() << endl;
}

const s_tool_command_argument lua_args[] =
{
	{ _tool_command_argument_type_file, L"Lua Script" },
};

static const s_tool_command lua_script_command
{
	L"lua",
	lua_proc,
	lua_args,
	ARRAYSIZE(lua_args),
	false
};
