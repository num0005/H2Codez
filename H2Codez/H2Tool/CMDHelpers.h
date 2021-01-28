#pragma once
#include "util/string_util.h"


/*
	Show a CMD prompt to the user
	Returns user response or default
*/
inline static bool prompt_user(const std::string& message, bool default = false)
{
	std::cout << message << " (Y/N) [Default:" << (default ? "Y" : "N") << "]" << std::endl;

	std::string input;
	std::cin >> input;
	str_trim(input);
	input = tolower(input);
	if (input.size() >= 1)
	{
		if (input[0] == 'y')
		{
			return true;
		}
		else if (input[0] == 'n')
		{
			return false;
		}
	}
	return default;
}

/*
	Show a CMD prompt to the user waiting till user gives valid response
	Returns user response
*/
inline static bool prompt_user_wait(const std::string & message)
{
	std::cout << message << " (Y/N)" << std::endl;
	while (std::cin)
	{
		std::string input;
		std::cin >> input;
		str_trim(input);
		input = tolower(input);
		if (input.size() >= 1)
		{
			if (input[0] == 'y')
			{
				return true;
			}
			else if (input[0] == 'n')
			{
				return false;
			}
		}
	}
	abort(); // unreachable
}
