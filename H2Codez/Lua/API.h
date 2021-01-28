#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <optional>

#define LUA_API_VERSION 1

class LuaAPI {
protected:
	LuaAPI(sol::state& _lua) :
		lua(_lua)
	{};

	sol::state& lua;
};


class LuaInterface {
public:
	LuaInterface();

	~LuaInterface() {
		for (auto api : apis)
			delete api;
	}

	std::optional<std::string> execute_script(const std::string& filename) {
		auto result = lua.safe_script_file(filename, sol::script_pass_on_error, sol::load_mode::text);
		if (!result.valid()) {
			sol::error err = result;
			return err.what();
		} else {
			return std::optional<std::string>();
		}
	}

private:
	template <typename API>
	inline void register_api() {
		static_assert(std::is_base_of<LuaAPI, API>::value, "API must inherit LuaAPI");

		apis.push_back(new API(lua));
	}

	sol::state lua;
	std::vector<LuaAPI*> apis;
};
