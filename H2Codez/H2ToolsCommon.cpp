#include <cwchar>
#include "stdafx.h"
#include "Patches.h"
#include "H2ToolsCommon.h"
#include <regex>


typedef int (WINAPI *LoadStringW_Typedef)(HINSTANCE hInstance, UINT uID, LPWSTR lpBuffer, int cchBufferMax);
LoadStringW_Typedef LoadStringW_Orginal;

typedef wchar_t* (WINAPI *GetCommandLineW_Typedef)();
GetCommandLineW_Typedef GetCommandLineW_Orginal;

static const wchar_t *map_types[] = 
{
	L"Single Player",
	L"Multiplayer",
	L"Main Menu",
	L"Multiplayer Shared",
	L"Single Player Shared"
};

int WINAPI LoadStringW_Hook(HINSTANCE hInstance, UINT uID, LPWSTR lpBuffer, int cchBufferMax)
{ 
	if ((310 <= uID && uID <= 318) && (GetModuleHandleW(L"H2alang") == hInstance)) {
		wcsncpy_s(lpBuffer, cchBufferMax, map_types[uID / 2 - 155], cchBufferMax);
		return std::wcslen(lpBuffer);
	}
	return LoadStringW_Orginal(hInstance, uID, lpBuffer, cchBufferMax);
}

wchar_t* __stdcall GetCommandLineW_Hook()
{
	wchar_t *real_cmd = GetCommandLineW_Orginal();
	std::wstring fake_cmd = std::regex_replace(real_cmd, std::wregex(L" pause_after_run"), L"");
	wcscpy(real_cmd, fake_cmd.c_str());
	return real_cmd;
}

bool H2CommonPatches::newInstance()
{
	TCHAR exePath[MAX_PATH];
	if (!GetModuleFileName(game.base, exePath, MAX_PATH))
		return false;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess(exePath, nullptr, nullptr, nullptr, false, INHERIT_PARENT_AFFINITY, nullptr, nullptr, &si, &pi);
	return true;
}

void H2CommonPatches::SendExitNotice()
{
	char msg[] = "cya!";
	LPTSTR lpszPipename1 = TEXT("\\\\.\\pipe\\h2tool_status");
	HANDLE hPipe1 = CreateFile(lpszPipename1, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	DWORD cbWritten;
	WriteFile(hPipe1, msg, strlen(msg), &cbWritten, NULL);
	CloseHandle(hPipe1);
}

void H2CommonPatches::Init()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	LoadStringW_Orginal = LoadStringW;
	DetourAttach(&(PVOID&)LoadStringW_Orginal, LoadStringW_Hook);

	GetCommandLineW_Orginal = GetCommandLineW;
	DetourAttach(&(PVOID&)GetCommandLineW_Orginal, GetCommandLineW_Hook);

	DetourTransactionCommit();
}