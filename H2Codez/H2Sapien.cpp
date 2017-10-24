#include "stdafx.h"
#include "H2Sapien.h"
#include "Patches.h"
#include "resource.h"

typedef HWND(__fastcall *create_main_window)(HMENU thisptr, int __unused, HWND hWndParent, HMENU hMenu, LPCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, HMENU oldmenu, LPVOID lpParam);
create_main_window create_main_window_orginal;

//bool __fastcall load_main_window(int thisptr,int unused, int a2, int a3, int a4, int *a5)
typedef bool(__fastcall *load_main_window)(int thisptr, int unused, int a2, int a3, int a4, int *a5);
load_main_window load_main_window_orginal;

typedef int (WINAPI *LoadStringW_Typedef)(HINSTANCE hInstance, UINT uID, LPWSTR lpBuffer, int cchBufferMax);
LoadStringW_Typedef LoadStringW_Orginal;

bool fake;
int WINAPI LoadStringW_Hook(HINSTANCE hInstance, UINT uID, LPWSTR lpBuffer, int cchBufferMax)
{
	if (uID == 421 || uID == 430 || uID == 431 || uID == 633 || uID == 763 || uID == 768 || uID == 4550 || uID == 4690 || uID == 4745) {
		fake = !fake;
	}

	if (uID == 4745) {
		fake = !fake;
	}
	return LoadStringW_Orginal(hInstance, uID, lpBuffer, cchBufferMax);
}

bool __fastcall load_main_window_hook(int thisptr, int unused, int a2, int a3, int a4, int *a5)
{
	int menu_ptr = thisptr + 12;
	HMENU menu = CAST_PTR(HMENU,menu_ptr);
	menu = LoadMenu(g_hModule, MAKEINTRESOURCE(SAPIEN_MENU));
	return load_main_window_orginal(thisptr, 0, a2, a3, a4, a5);
}

HWND __fastcall create_main_window_hook(HMENU thisptr, int __unused, HWND hWndParent, HMENU hMenu, LPCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, HMENU oldmenu, LPVOID lpParam)
{
	HMENU new_menu = LoadMenu(g_hModule, MAKEINTRESOURCE(SAPIEN_MENU));
	return create_main_window_orginal(thisptr, 0, hWndParent, hMenu, lpWindowName, dwStyle, dwExStyle, new_menu, lpParam);
}

void H2SapienPatches::Init()
{
#pragma region Patches
	// stop the default menu overwriting our custom one
	NopFill(0x47AD09, 0x15);

	BYTE patch_can_display_tag[] = { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }; // mov eax, 1; retn
	WriteBytesASM(0x630200, patch_can_display_tag, sizeof(patch_can_display_tag));
#pragma endregion

#pragma region Hooks

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	create_main_window_orginal =CAST_PTR(create_main_window,0x469030);
	DetourAttach(&(PVOID&)create_main_window_orginal, create_main_window_hook);

	load_main_window_orginal = CAST_PTR(load_main_window,0x47ACE0);
	DetourAttach(&(PVOID&)load_main_window_orginal, load_main_window_hook);

	LoadStringW_Orginal = LoadStringW;
	DetourAttach(&(PVOID&)LoadStringW_Orginal, LoadStringW_Hook);

	DetourTransactionCommit();
#pragma endregion
}