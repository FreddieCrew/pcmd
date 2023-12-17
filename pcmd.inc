#if defined _inc_BOILERPLATE_INCLUDE
	#undef _inc_PCMD_INCLUDE
#endif
#if defined _PCMD_INCLUDE_included
	#endinput
#endif
#define _PCMD_INCLUDE_included

#tryinclude <a_samp>

// Natives
native PCMD_COMMAND(playerid, cmdtext[]);

// Macros
#define CMD:%0(%1) \
			forward _%0(%1); \
			public _%0(%1)

#define PCMD:%1(%2) \ 
					  CMD:%1(%2)

#define COMMAND:%1(%2) \ 
					  CMD:%1(%2)

// Callback hooks
public OnPlayerCommandText(playerid, cmdtext[]){
    
    #if defined pcmd_OnPlayerCommandText
		forward pcmd_OnPlayerCommandText(playerid, cmdtext[]);
	#endif
	return PCMD_COMMAND(playerid, cmdtext);
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

// Forwards
forward OnPlayerCommandReceived(playerid, cmdtext[]);
forward OnPlayerCommandPerformed(playerid, cmdtext[], success);
