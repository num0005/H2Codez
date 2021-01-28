#include "API.h"
#include "TagAPI.h"
#include "Version.h"

LuaInterface::LuaInterface() {
	lua.open_libraries();
	lua["H2CODEZ_VERSION"] = version;
	lua["API_VERSION"] = LUA_API_VERSION;

	lua["BUILD_DATE"] = __DATE__;
	lua["BUILD_TIME"] = __TIME__;
	register_api<LuaTagAPI>();
}