#include "stdafx.h"
#include "H2Sapien.h"
#include "H2ToolsCommon.h"
#include "HaloScriptInterface.h"
#include "Patches.h"
#include "resource.h"
#include <Shellapi.h>
#include <iostream>
#include <fstream>
#include <D3D9.h>

using namespace HaloScriptCommon;

typedef int (__thiscall *WTL_CWindow_Input)(void *thisptr, int a2, UINT uMsg, int hMenu, LPARAM lParam, int *a6, int a7);
WTL_CWindow_Input main_window_input_orginal;

typedef HMENU(WINAPI *LoadMenuTypedef)(_In_opt_ HINSTANCE hInstance, _In_ LPCWSTR lpMenuName);
static LoadMenuTypedef LoadMenuOrginal;

bool run_script(char *script_text);

char command_script[0x100];
INT_PTR CALLBACK SapienRunCommandProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE) {
		EndDialog(hwndDlg, 0);
		return true;
	}
	if (uMsg == WM_COMMAND && LOWORD(wParam)) {
		if (GetDlgItemText(hwndDlg, SAPIEN_COMMAND, command_script, sizeof(command_script)))
			run_script(command_script);
		EndDialog(hwndDlg, 0);
		return true;
	}
	return false;
}

void **script_epilog(void *a1, int return_data)
{
	auto script_epilog_impl = reinterpret_cast<void**(__cdecl *)(void *a1, int return_data)>(0x52CC70);
	return script_epilog_impl(a1, return_data);
}

HMENU new_menu;

static HMENU WINAPI LoadMenuHook(_In_opt_ HINSTANCE hInstance, _In_ LPCWSTR lpMenuName)
{
	if (hInstance == GetModuleHandle(NULL)) {
		return new_menu;
	}
	return LoadMenuOrginal(hInstance, lpMenuName);
}

bool running_game_scripts = false;
bool using_in_game_settings = false;

inline static void CheckItem(UINT item, bool enable)
{
	CheckMenuItem(new_menu, item, MF_BYCOMMAND | (enable ? MF_CHECKED : MF_UNCHECKED));
}

video_settings halo2_video_settings;
video_settings sapien_defaults;

void apply_video_settings()
{
	CheckItem(SAPIEN_IN_GAME_LOD, using_in_game_settings);

	WriteValue(0x00A5D134, using_in_game_settings ? halo2_video_settings : sapien_defaults);
}

int __fastcall main_window_input_hook(void *thisptr, BYTE _, int a2, UINT uMsg, int wParam, LPARAM lParam, int *subfunction_out, int handled)
{
	if (uMsg == WM_COMMAND && !handled) {
		switch (wParam) {
			case SAPIEN_FILE_NEWINSTANCE:
			{
				return H2CommonPatches::newInstance();
			}
			case SAPIEN_OPEN_RUN_COMMAND_DIALOG:
			{
				DialogBoxParam(g_hModule, MAKEINTRESOURCE(SAPIEN_COMMAND_DIALOG), 0, SapienRunCommandProc, 0);
				return 1;
			}
			case SAPIEN_SCRIPT_DOC:
			{
				H2CommonPatches::generate_script_doc();
				return 1;
			}
			case SAPIEN_IN_GAME_LOD:
			{
				typedef void(__cdecl *update_video_settings)(char a1);
				update_video_settings update_video_settings_impl = reinterpret_cast<update_video_settings>(0x006FBCF0);

				using_in_game_settings = !using_in_game_settings;
				apply_video_settings();

				conf.setBoolean("in_game_lod", using_in_game_settings);

				update_video_settings_impl(0);

				return 1;
			}
			case 32837:
			{
				char expert_mode = *CAST_PTR(char*, 0xA68319);
				conf.setBoolean("expert_mode", expert_mode);
				break;
			}
			case 32870:
			{
				running_game_scripts = !running_game_scripts;
				conf.setBoolean("running_game_scripts", running_game_scripts);
				CheckItem(32870, running_game_scripts);
				break;
			}
		}
	}
	return main_window_input_orginal(thisptr, a2, uMsg, wParam, lParam, subfunction_out, handled);
}
 
bool is_ctrl_down()
{
	return HIBYTE(GetKeyState(VK_CONTROL));
}

typedef char(__cdecl *print_to_console)(char *Format);

void __stdcall on_console_input(WORD keycode)
{
	printf("key  :  %d\n", keycode);

	char *console_input = reinterpret_cast<char*>(0xA9F52C);
	WORD *cursor_pos = reinterpret_cast<WORD*>(0xa9f636);
	auto print_console = reinterpret_cast<print_to_console>(0x00616720);

	printf("console: %s \n", console_input);

	switch (keycode) {
	case 46: // delete key
		ZeroMemory(console_input, 0x100);
		*cursor_pos = 0;
		print_console("cleared console.");
		break;
	case 'C':
		if (is_ctrl_down()) {
			HWND *main_hwnd = reinterpret_cast<HWND *>(0x00A68B9C);
			if (H2CommonPatches::copy_to_clipboard(console_input, *main_hwnd))
				print_console("copied to clipboard!");
		}
		break;
	case 'V':
		std::string new_text;
		HWND *main_hwnd = reinterpret_cast<HWND *>(0x00A68B9C);
		if (is_ctrl_down() && H2CommonPatches::read_clipboard(new_text, *main_hwnd)) {
			*cursor_pos = static_cast<WORD>(new_text.size());
			strncpy(console_input, new_text.c_str(), 0x100);
			print_console("pasted from clipboard!");
		}
		break;
	}

}

// hooks a switch statement that handles speical key presses (e.g. enter, tab)
__declspec(naked) void console_input_jump_hook()
{
	__asm {
		// save register
		push eax
		push ecx
		push edx

		// undo add
		sub eax, 0xFFFFFFF7
		// pass eax (keycode) to our code
		push eax
		call on_console_input

		// restore registers
		pop edx
		pop ecx
		pop eax

		// replaced code
		cmp eax, 254

		// jump back to sapien code
		ret
	}
}

int console_write_return_addr;
int memcpy_impl = 0x4ADDC0;
// hooks the function that handles writing keypresses to console buffer if printable
__declspec(naked) void console_write_hook()
{
	__asm {
		// backup the return address
		pop eax
		mov console_write_return_addr, eax

		// replaced code
		call memcpy_impl
		
		// get keycode and check
		mov al, [ebx + 1]
		cmp al, 0x60 // ascii '`'

		// ignore input
		jz ignore_input
		// return to normal execution
		jmp end_function

		ignore_input:
		add esp, 0x18
		// push new return addr pointing to function epilog
		mov console_write_return_addr, 0x58F85E

	end_function:
		jmp console_write_return_addr
	}
}

bool run_script(char *script_text)
{
	bool return_data;
	int run_command_console = 0x4EC020;
	int esp_backup;

	__asm {
		mov esp_backup, esp
		push 1
		mov	esi, script_text
		call run_command_console
		mov return_data, al
		mov esp, esp_backup
	}
	return return_data;
}

std::string baggage_name;

errno_t __cdecl fopen_s_baggage_hook(FILE **File, const char *Filename, const char *Mode)
{
	baggage_name = H2CommonPatches::get_temp_name("baggage.txt");
	return fopen_s(File, baggage_name.c_str(), "w");
}

int __cdecl fclose_baggage_hook(FILE *File)
{
	int ret_data = fclose(File);
	ShellExecuteA(NULL, NULL, baggage_name.c_str(), NULL, NULL, SW_SHOW);
	return ret_data;
}

errno_t print_help_to_doc()
{
	H2CommonPatches::generate_script_doc("hs_doc.txt");
	return 0;
}

void **__cdecl status_func_impl(int command_id, void *a2, char a3)
{
	ofstream output;
	std::string temp_file_name = H2CommonPatches::get_temp_name("status.txt");

	output.open(temp_file_name, ios::out);
	if (output)
	{
		for (hs_global_variable *current_var : g_halo_script_interface->global_table)
		{
			std::string value_as_string = get_value_as_string(current_var->variable_ptr, current_var->type);
			output << current_var->name << "   :    " << value_as_string << std::endl;
		}
	}
	output.close();
	ShellExecuteA(NULL, NULL, temp_file_name.c_str(), NULL, NULL, SW_SHOW);
	return HaloScriptCommon::epilog(a2, 0);
}

signed int get_tick_rate()
{
	WORD *tick_rate = reinterpret_cast<WORD*>(0x00A6C700);
	return *tick_rate;
}

bool __cdecl scripts_disabled()
{
	return !running_game_scripts;
}

void __cdecl process_decoration_brush_input__hook(int *a1, int *a2, char left_pressed, char right_pressed)
{
	typedef void(__cdecl *process_decoration_brush_input)(int *a1, int *a2, char left_pressed, char right_pressed);
	process_decoration_brush_input process_decoration_brush_input_impl = reinterpret_cast<process_decoration_brush_input>(0x4A8ED0);

	process_decoration_brush_input_impl(a1, a2, left_pressed, right_pressed);

	if (left_pressed || right_pressed)
		WriteValue(0xFE8CE4, 1); // not sure how but this reset something and makes the new decor render correctly.
}

char __cdecl set_scenario_path_hook(LPCWSTR path_passed_to_us)
{
	typedef char (__cdecl *set_scenario_path)(LPCWSTR path);
	auto set_scenario_path_impl = reinterpret_cast<set_scenario_path>(0x00458E40);
	if (!set_scenario_path_impl(path_passed_to_us)) // support standard path escaping
	{
		int argwc = *reinterpret_cast<int*>(0x010E6A7C);
		wchar_t **argwv = *reinterpret_cast<wchar_t ***>(0x010E6A84);
		if (argwc >= 2)
			return set_scenario_path_impl(argwv[1]);
		return false;
	}
	return true;
}


hs_command status_cmd(
	"status",
	hs_type::nothing,
	hs_default_func_check,
	status_func_impl,
	"dumps the value of all global status variables to file."
);

template <typename value_type>
inline void GetHalo2DisplaySetting(const char *name, value_type &result)
{
	HKEY video_settings;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Halo 2\\Video Settings", 0, KEY_READ, &video_settings) == ERROR_SUCCESS) {
		DWORD size = sizeof(DWORD);
		DWORD value;
		if (RegGetValueA(video_settings, NULL, name, RRF_RT_REG_DWORD, NULL, &value, &size) == ERROR_SUCCESS)
			result = static_cast<value_type>(value);
		RegCloseKey(video_settings);
	}
}

void InitHalo2DisplaySettings()
{
	GetHalo2DisplaySetting("AntiAliasing", halo2_video_settings.AntiAliasing);
	GetHalo2DisplaySetting("AspectRatio", halo2_video_settings.AspectRatio);
	GetHalo2DisplaySetting("Brightness", halo2_video_settings.Brightness);
	GetHalo2DisplaySetting("DisplayMode", halo2_video_settings.DisplayMode);
	GetHalo2DisplaySetting("Gamma", halo2_video_settings.Gamma);
	GetHalo2DisplaySetting("HubArea", halo2_video_settings.HubArea);
	GetHalo2DisplaySetting("LevelOfDetail", halo2_video_settings.LevelOfDetail);
	GetHalo2DisplaySetting("SafeArea", halo2_video_settings.SafeArea);
	GetHalo2DisplaySetting("ScreenRefresh", halo2_video_settings.ScreenInfo.refresh_rate);
	GetHalo2DisplaySetting("ScreenResX", halo2_video_settings.ScreenInfo.x);
	GetHalo2DisplaySetting("ScreenResY", halo2_video_settings.ScreenInfo.y);
}

void H2SapienPatches::Init()
{
	// set current directory to executable path
	std::wstring new_current_dir = H2CommonPatches::GetExeDirectory();
	SetCurrentDirectoryW(new_current_dir.c_str());
#pragma region value init
	hs_command **command_table = reinterpret_cast<hs_command **>(0x9E9E90);
	hs_global_variable **global_table = reinterpret_cast<hs_global_variable **>(0x9ECE28);
	g_halo_script_interface->init_custom(command_table, global_table);
	g_halo_script_interface->RegisterCommand(hs_opcode::status, &status_cmd);

	new_menu = LoadMenu(g_hModule, MAKEINTRESOURCE(SAPIEN_MENU));
	InitHalo2DisplaySettings();

	using_in_game_settings = conf.getBoolean("in_game_lod", 0);
	WriteValue(0xA68319, (BYTE)conf.getBoolean("expert_mode", 1)); // set is_expert_mode to one

	running_game_scripts = conf.getBoolean("running_game_scripts", 0);
	CheckItem(32870, running_game_scripts);

	WriteValue(0x00F84D30, conf.getNumber("CPUScore", 5.0));
	WriteValue(0x00F84D28, conf.getBoolean("AllowVsync", 0));
	WriteValue(0x00F84D24, conf.getBoolean("CinematicShadow", 0));

	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	int SystemMemory = static_cast<int>(statex.ullAvailPhys / (1024 * 1024));
	int VideoMemory = conf.getNumber("VideoMemory", 100) * 1024 * 1024; // megabytes --> bytes
	int use_hardware_vertexprocessing = conf.getBoolean("use_hardware_vertexprocessing", true);
	WriteValue(0xF84D1C, SystemMemory);
	WriteValue(0xF84D18, VideoMemory);
	WriteValue(0xF84D10, use_hardware_vertexprocessing);
	apply_video_settings();
#pragma endregion

#pragma region Patches
	// Stop sapien from getting a mutex on the main directory
	NopFill(0x409D3D, 0xD);

	// replace bultin IO since it seems to crash
	PatchCall(0x00477AE8, fopen_s_baggage_hook);
	PatchCall(0x00477D4F, fclose_baggage_hook);

	PatchCall(0x00477C05, fwprintf); // scenario name
	PatchCall(0x00477C54, fwprintf);
	PatchCall(0x00477D25, fwprintf); // ==block sizes==
	PatchCall(0x00477D45, fwprintf);

	// %ws should really be used for wchar_t strings instead of %s
	WritePointer(0x477C4F, L"%ws\n\n");
	WritePointer(0x477D40, L"%ws\n");

	PatchCall(0x5783B0, print_help_to_doc);
	WriteCallTo(0x4ECC2E, &console_input_jump_hook);
	// replace a call to memcpy
	PatchCall(0x58F6AA, &console_write_hook);

	// don't clear the console contents when closed
	NopFill(0x4ECD7C, 5);

	// allow other processes to read files open with fopen_s
	WriteValue(0x00738FF3 + 1, _SH_DENYWR);

	// fixes debug_tags command messing up string formating
	// and getting a werid path directly in program files
	const char *tag_debug_format = "%S//%s_tag_dump.txt";
	WriteValue(0x004B5F33 + 1, tag_debug_format);

	// Replace pointers to the commmand table
	static DWORD cmd_offsets[] = 
	{
		0x004E2225 + 3, 0x004E23F0 + 3,  0x004E2414 + 3,
		0x004E2701 + 3, 0x004E2DF4 + 3, 0x004E3014 + 3,
		0x004E304D,     0x004E30FB
	};

	hs_command **cmds = g_halo_script_interface->get_command_table();

	for (DWORD addr : cmd_offsets)
		WritePointer(addr, cmds);

	// patch command table size
	const static int hs_cmd_table_size = g_halo_script_interface->get_command_table_size();
	WriteValue(0x008EB118, hs_cmd_table_size);

	// Replace pointers to the globals table

	static DWORD var_offsets[] =
	{
		0x004E2295, 0x004E22B0, 0x004E22F0,
		0x004E2334, 0x004E27B1, 0x00635A11,
		0x00635A2D, 0x00635A7D, 0x00635AFB,
		0x00635B12
	};

	hs_global_variable **vars = g_halo_script_interface->get_global_table();

	for (DWORD addr : var_offsets)
		WriteValue(addr + 3, vars);

	// patch globals table size
	const static int hs_global_table_size = g_halo_script_interface->get_global_table_size();
	WriteValue(0x008EFDB4, hs_global_table_size);

	// fix "game_tick_rate"
	WriteJmpTo(0x006F7D60, get_tick_rate);

	WriteValue(0x00A5D104, sapien_defaults);

	PatchCall(0x0052C516, scripts_disabled);

	if (conf.getBoolean("decoration_force_update", false))
		PatchCall(0x004876DE, process_decoration_brush_input__hook);

	// fix sapien not working well with shell
	PatchCall(0x0046361A, set_scenario_path_hook);

	// disable this for now
	// Don't force display mode to 1
	//NopFill(0x006FBFF4, 0x16);
#pragma endregion

#pragma region Hooks

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	main_window_input_orginal = CAST_PTR(WTL_CWindow_Input, 0x475B60);
	DetourAttach(&(PVOID&)main_window_input_orginal, main_window_input_hook);

	LoadMenuOrginal = LoadMenuW;
	DetourAttach(&(PVOID&)LoadMenuOrginal, LoadMenuHook);

	DetourTransactionCommit();
#pragma endregion
}