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

	/* Escape a string so that it can be used in a Lua expression. */
	inline static std::string escape_string_for_lua(const char* string, const size_t length) {
		std::string escaped_string = "\"";
		for (size_t i = 0; i < length; i++) {
			const char c = string[i];
			int character = static_cast<int>(c);
			if (c == '\"')
				escaped_string += "\\\"";
			else if (c == '\n')
				escaped_string += "\\\n";
			else if (c == '\r')
				escaped_string += "\\\r";
			else if (c == '\t')
				escaped_string += "\\\t";
			else if (c == '\\')
				escaped_string += "\\\\";
			else if (isprint(character))
				escaped_string += c;
			else
				escaped_string += "\\" + std::to_string(character);

		}
		return escaped_string + "\"";
	}

	template <typename T>
	inline static std::string to_lua_syntax(const T& val) {
		return std::to_string(val);
	}

	template <>
	inline static std::string to_lua_syntax(const std::string& val) {
		return escape_string_for_lua(val.c_str(), val.length());
	}

	
	inline static std::string to_lua_syntax(const char* val) {
		// you better null terminate that string, or else!
		return escape_string_for_lua(val, strlen(val));
	}

	template <size_t length>
	inline static std::string to_lua_syntax(const char (&val)[length]){
		return escape_string_for_lua(val, strlen_s(val, length));
	}

	// Helper function
	template <typename T, typename... Args>
	inline static void stringify_impl(std::string& string, const std::string& name, T value, Args ...args) {
		string += " " + name + " = " + to_lua_syntax(value);

		if constexpr (sizeof...(Args) != 0) {
			string += ",";
			stringify_impl(string, std::forward<Args>(args)...);
		}
	}

	/* Converts the arguments list to Lua table syntax
	* stringify("a", 1, "test", 1.0, "str", "hello world!") == "{ a = 1, test = 1.0, str = \"hello world\"}"
	*/
	template <typename... Args>
	inline static std::string stringify(Args ...args) {
		static_assert(sizeof...(Args) % 2 == 0, "Bad arguments");
		if constexpr (sizeof...(Args) == 0)
			return "";
		std::string str = "{";
		stringify_impl(str, std::forward<Args>(args)...);
		str += " }";
		return str;
	}
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
