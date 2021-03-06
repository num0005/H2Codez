#pragma once
#include "hs_types.h"
#include "stdafx.h"

namespace HaloScriptCommon
{
	typedef char(__cdecl *func_check)(__int16 opcode, void *DatumIndex);
	typedef void **(__cdecl *func_impl)(int opcode, void *DatumIndex, char user_cmd);

	struct hs_command
	{
		hs_type return_type;
		char *name;
		DWORD unk1 = 0;
		func_check check_command_args;
		func_impl command_impl;
		char *desc;
		char *usage;
		WORD arg_count = 0;
		WORD arg_type_array[];

		hs_command(char *cmd_name,
			hs_type ret_type,
			const func_check arg_check,
			const func_impl impl,
			char *_desc = nullptr,
			char *_usage = nullptr) :
			name(cmd_name),
			return_type(ret_type),
			check_command_args(arg_check),
			command_impl(impl),
			desc(_desc),
			usage(_usage)
		{
		}

	};
	static_assert(sizeof(hs_command) == 0x20, "Bad struct size");

	inline hs_command *NewCommand(char *cmd_name,
		hs_type ret_type,
		const func_check arg_check,
		const func_impl impl,
		char *_desc = nullptr,
		char *_usage = nullptr,
		int _arg_count = 0,
		const hs_type *arg_types = nullptr)
	{
		hs_command *cmd = static_cast<hs_command*>(malloc(sizeof(hs_command) + (_arg_count * sizeof(WORD))));
		cmd->name = cmd_name;
		cmd->return_type = ret_type;
		cmd->check_command_args = arg_check;
		cmd->command_impl = impl;
		cmd->desc = _desc;
		cmd->usage = _usage;
		cmd->arg_count = _arg_count;
		if (arg_types && _arg_count > 0)
		{
			for (int i = 0; i < _arg_count; i++)
				cmd->arg_type_array[i] = static_cast<WORD>(arg_types[i]);
		}
		return cmd;
	}
}