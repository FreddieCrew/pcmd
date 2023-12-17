#include <a_samp>
#include <pcmd>

main() {
	print("Hello");
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success) {
	if(!success) {
		SendClientMessage(playerid, -1, "Invalid command");
	}
	printf("playerid: %d | Command: %s | Success: %s", playerid, cmdtext, (success) ? ("Success") : ("Fail"));
	return 1;
}

CMD:test(playerid) {
    SendClientMessage(playerid, -1, "Hello, world!");
    return 1;
}
