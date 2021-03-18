#include "API.h"
#include "BlamTypes.h"
#include "TagAPI.h"
#include "Version.h"
#include <lfs\lfs.h>

LuaInterface::LuaInterface() {
	lua.open_libraries();
	lua.require("lfs", luaopen_lfs);

	lua["H2CODEZ_VERSION"] = version;
	lua["API_VERSION"] = LUA_API_VERSION;

	lua["BUILD_DATE"] = __DATE__;
	lua["BUILD_TIME"] = __TIME__;

	register_api<BasicTypesAPI>();
	register_api<LuaTagAPI>();
}