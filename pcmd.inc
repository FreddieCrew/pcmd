#if defined _inc_BOILERPLATE_INCLUDE
	#undef _inc_PCMD_INCLUDE
#endif
#if defined _PCMD_INCLUDE_included
	#endinput
#endif
#define _PCMD_INCLUDE_included

enum CMD_RETURN {
	PCMD_NOT_FOUND = 0,
	PCMD_SUCCESS = 1,
	PCMD_FAIL = -1
};

// Natives
native PCMD_COMMAND(playerid, cmdtext[]);

// Forwards
forward OnPlayerCommandReceived(playerid, cmdtext[]);
forward OnPlayerCommandPerformed(playerid, cmdtext[], CMD_RETURN:success);

// Macros
#define CMD:%0(%1) \
			forward _%0(%1); \
			public _%0(%1)

#define PCMD:%1(%2) \ 
					  CMD:%1(%2)

#define COMMAND:%1(%2) CMD:%1(%2)

// Callback hooks
public OnPlayerCommandText(playerid, cmdtext[]) {
	PCMD_COMMAND(playerid, cmdtext);

	#if defined pcmd_OnPlayerCommandText
		pcmd_OnPlayerCommandText(playerid, cmdtext);
	#else
		return 1;
	#endif
	#if defined OnPlayerCommandPerformed
		return OnPlayerCommandPerformed(playerid, cmdtext, CMD_RETURN:PCMD_NOT_FOUND);
	#else
		return CallLocalFunction("OnPlayerCommandPerformed", "isi", playerid, cmdtext, 0);
	#endif
}

#if defined _ALS_OnPlayerCommandText
	#undef OnPlayerCommandText
#else
	#define _ALS_OnPlayerCommandText
#endif
#define OnPlayerCommandText pcmd_OnPlayerCommandText
#if defined pcmd_OnPlayerCommandText
	forward pcmd_OnPlayerCommandText(playerid, cmdtext[]);
#endif
