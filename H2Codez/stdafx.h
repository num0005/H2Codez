// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:

#pragma region WindowsSdks

#include <Windows.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <io.h>
#include <string>
#include <stdio.h>
#include <list>
#include <fstream>
#include <stdarg.h>
#include <tchar.h>
#include <filesystem>
#pragma endregion

#include "h2codez.h"
#include "Detours\detours.h"
#include "Logs.h"
#include "H2Tool\H2Tool_Commands.h"
#include "discord-rpc.h"
#include "Patches.h"
#include "Settings.h"

extern Settings conf;
extern BOOL EnableDbgConsole;
extern char app_directory[256];


#pragma comment( lib, "psapi.lib" )



#pragma region NameSpaces

using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::dec;
using std::to_string;
using std::ofstream;


#pragma endregion

extern HMODULE g_hModule;
