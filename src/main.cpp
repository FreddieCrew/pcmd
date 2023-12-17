#include <stddef.h>
#include <cstdint>
#include <cstdio>

#include <amx/amx.h>
#include <plugincommon.h>
#include <unordered_map>

#include "common.hpp"
#include "natives.hpp"

logprintf_t logprintf;

#define MAX_LENGTH (17)

struct PCMD_CMD_AMX {
	AMX* amx;
	int CommandReceived, CommandPerformed;
}

AMX_funcs[MAX_LENGTH];
int AMX_l;


cell Natives::PCMD_COMMAND(AMX* amx, cell* params) {
	cell* addr;
	amx_GetAddr(amx, params[2], &addr);
	amx_StrLen(addr, &len);
	char cmdtext[128];
	amx_GetString(cmdtext, addr, 0, len);
	cmdtext[0] = '_';

	int pos = 0, cmd_end;
	do {
		++pos;
		if (('A' <= cmdtext[pos]) && (cmdtext[pos] <= 'Z'))
			cmdtext[pos] += ('a' - 'A');
		else if (cmdtext[pos] == '\0')
			break;
		else if (cmdtext[pos] == ' ') {
			cmd_end = pos;
			cmdtext[pos++] = '\0';
			goto breakpoint;
		}
	} while (1);
	cmd_end = 0;

breakpoint:
	int pubidx;
	cell retval{}, params_addr;
	int i;

	for (i = 0; i <= AMX_l; ++i) {
		if ((AMX_funcs[i].amx != NULL) && (amx_FindPublic(AMX_funcs[i].amx, cmdtext, &pubidx) == AMX_ERR_NONE)) {
			if (AMX_funcs[i].CommandReceived != 0x7FFFFFFF) {
				cmdtext[0] = '/';
				if (cmd_end > 0)
					cmdtext[cmd_end] = ' ';
				amx_PushString(AMX_funcs[i].amx, &params_addr, 0, cmdtext, 0, 0);
				amx_Push(AMX_funcs[i].amx, params[1]);
				amx_Exec(AMX_funcs[i].amx, &retval, AMX_funcs[i].CommandReceived);
				amx_Release(AMX_funcs[i].amx, params_addr);

				if (retval == 0)
					return true;
				cmdtext[0] = '_';
				if (cmd_end > 0)
					cmdtext[cmd_end] = ' ';
			}

			while (cmdtext[pos] == ' ') pos++;
			amx_PushString(AMX_funcs[i].amx, &params_addr, 0, cmdtext + pos, 0, 0);
			amx_Push(AMX_funcs[i].amx, params[1]);
			amx_Exec(AMX_funcs[i].amx, &retval, pubidx);
			amx_Release(AMX_funcs[i].amx, params_addr);

			if (AMX_funcs[i].CommandPerformed != 0x7FFFFFFF) {
				cmdtext[0] = '/';
				if (cmd_end > 0)
					cmdtext[cmd_end] = ' ';
				amx_Push(AMX_funcs[i].amx, retval);
				amx_PushString(AMX_funcs[i].amx, &params_addr, 0, cmdtext, 0, 0);
				amx_Push(AMX_funcs[i].amx, params[1]);
				amx_Exec(AMX_funcs[i].amx, &retval, AMX_funcs[i].CommandPerformed);
				amx_Release(AMX_funcs[i].amx, params_addr);
			}

			return 1;
		}
	}
	try {
		for (i = 0; i <= AMX_l; ++i) {
			if ((AMX_funcs[i].amx != NULL)) {
				if (AMX_funcs[i].CommandReceived != 0x7FFFFFFF) {
					cmdtext[0] = '/';
					if (cmd_end > 0)
						cmdtext[cmd_end] = ' ';
					amx_PushString(AMX_funcs[i].amx, &params_addr, 0, cmdtext, 0, 0);
					amx_Push(AMX_funcs[i].amx, params[1]);
					amx_Exec(AMX_funcs[i].amx, &retval, AMX_funcs[i].CommandReceived);
					amx_Release(AMX_funcs[i].amx, params_addr);
					if (retval == 0)
						return true;
					cmdtext[0] = '_';
					if (cmd_end > 0)
						cmdtext[cmd_end] = ' ';
				}
				while (cmdtext[pos] == ' ') pos++;
				amx_PushString(AMX_funcs[i].amx, &params_addr, 0, cmdtext + pos, 0, 0);
				amx_Push(AMX_funcs[i].amx, params[1]);
				amx_Exec(AMX_funcs[i].amx, &retval, pubidx);
				amx_Release(AMX_funcs[i].amx, params_addr);
				if (AMX_funcs[i].CommandPerformed != 0x7FFFFFFF) {
					cmdtext[0] = '/';
					if (cmd_end > 0)
						cmdtext[cmd_end] = ' ';
					amx_Push(AMX_funcs[i].amx, retval);
					amx_PushString(AMX_funcs[i].amx, &params_addr, 0, cmdtext, 0, 0);
					amx_Push(AMX_funcs[i].amx, params[1]);
					amx_Exec(AMX_funcs[i].amx, &retval, AMX_funcs[i].CommandPerformed);
					amx_Release(AMX_funcs[i].amx, params_addr);
				}
				return 1;
			}
		}

		if (AMX_funcs[0].CommandPerformed != 0x7FFFFFFF) {
			cmdtext[0] = '/';
			if (cmd_end > 0)
				cmdtext[cmd_end] = ' ';
			amx_Push(AMX_funcs[0].amx, -1);
			amx_PushString(AMX_funcs[0].amx, &params_addr, 0, cmdtext, 0, 0);
			amx_Push(AMX_funcs[0].amx, params[1]);
			amx_Exec(AMX_funcs[0].amx, &retval, AMX_funcs[0].CommandPerformed);
			amx_Release(AMX_funcs[0].amx, params_addr);

			if (retval == 1) {
				return false;
			}
		}

	} catch (const std::exception& e) {
		logprintf("[PCMD]: Exepction occured on %s: %s", __func__, e.what());
	}
	return 1;
}


AMX_NATIVE_INFO PluginNatives[] = {
	{"PCMD_COMMAND", Natives::PCMD_COMMAND},
	{NULL, NULL},
};

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx) {
	int idx;
	if (AMX_funcs[0].amx == NULL && amx_FindPublic(amx, "OnGameModeInit", &idx) == AMX_ERR_NONE)
		idx = 0;
	else
		idx = ++AMX_l;
	AMX_funcs[idx].amx = amx;
	amx_FindPublic(amx, "OnPlayerCommandReceived", &AMX_funcs[idx].CommandReceived);
	amx_FindPublic(amx, "OnPlayerCommandPerformed", &AMX_funcs[idx].CommandPerformed);
	return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void** ppData) {
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf((char*)"PCMD loaded");
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	logprintf((char*)"PCMD unloaded");
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
	int idx = 0;
	if (amx == AMX_funcs[0].amx)
		AMX_funcs[0].amx = NULL;
	else {
		for (idx = 1; idx < MAX_LENGTH; ++idx)
			if (AMX_funcs[idx].amx == amx) {
				if (AMX_l > 1)
					AMX_funcs[idx].amx = AMX_funcs[AMX_l].amx;
				AMX_funcs[AMX_l].amx = NULL;
				--AMX_l;
				break;
			}
	}
	return AMX_ERR_NONE;
}
