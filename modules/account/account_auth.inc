#if defined _inc_guard_account_auth
    #endinput
#endif

#define _inc_guard_account_auth

#include <a_samp>
#include <requests>
#include <PawnPlus>

#include <dialog_ids>
#include <database>

stock account_authenticate(const playerid) {
	new name[MAX_PLAYER_NAME + 1];
	GetPlayerName(playerid, name);

	task_yield(1);
	new Node:data = Node:await db_retrieveAccount(name);

	if(JsonNodeType(data) == JSON_NODE:JSON_NODE_NULL) {
		ShowPlayerDialog(playerid, DLG_ACCOUNT_AUTH_REGISTER, DIALOG_STYLE_PASSWORD, "Account Register", "Enter your password to register:", "Confirm", "Cancel");
	} else {
		ShowPlayerDialog(playerid, DLG_ACCOUNT_AUTH_LOGIN, DIALOG_STYLE_PASSWORD, "Account Login", "Enter your password to login:", "Confirm", "Cancel");
	}
	return 1;
}

#include <YSI_Coding\y_hooks>

hook OnGameModeInit() {
	print("INFO: modules/account_auth loaded");
	return 1;
}

hook OnPlayerConnect(playerid) {
	account_authenticate(playerid);
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	switch(dialogid) {
		case DLG_ACCOUNT_AUTH_REGISTER: {
			if(response) {
				if(isnull(inputtext) || strlen(inputtext) > 127) {
					ShowPlayerDialog(playerid, DLG_ACCOUNT_AUTH_REGISTER, DIALOG_STYLE_PASSWORD, "Account Register", "Password is invalid (empty or longer than 127 characters)\nRe-enter your password to register:", "Confirm", "Cancel");
					return Y_HOOKS_BREAK_RETURN_1;
				}

				new name[MAX_PLAYER_NAME + 1];
				GetPlayerName(playerid, name);

				new Node:data = JsonObject(
					"name", JsonString(name),
					"password", JsonString(inputtext)
				);

				task_yield(Y_HOOKS_BREAK_RETURN_1);
				await db_addAccount(data);
				account_authenticate(playerid);
			} else {
				Kick(playerid);
			}
			return Y_HOOKS_BREAK_RETURN_1;
		}
		case DLG_ACCOUNT_AUTH_LOGIN: {
			if(response) {
				if(isnull(inputtext) || strlen(inputtext) > 127) {
					ShowPlayerDialog(playerid, DLG_ACCOUNT_AUTH_LOGIN, DIALOG_STYLE_PASSWORD, "Account Login", "Password is invalid (empty or longer than 127 characters)\nRe-enter your password to login:", "Confirm", "Cancel");
					return Y_HOOKS_BREAK_RETURN_1;
				}

				new hash[BCRYPT_HASH_LENGTH],
					name[MAX_PLAYER_NAME + 1];

				task_yield(Y_HOOKS_BREAK_RETURN_1);
				GetPlayerName(playerid, name);
				new Node:data = Node:await db_retrieveAccount(name);
				JsonGetString(data, "password", hash);
				new bool:equal = bool:await db_bcryptVerify(hash, inputtext);

				if(equal) {
					ShowPlayerDialog(playerid, DLG_CHARACTER_PANEL, DIALOG_STYLE_LIST, "Character Panel", "Click here to setup a new character <", "Select", "Cancel");
				} else {
					ShowPlayerDialog(playerid, DLG_ACCOUNT_AUTH_LOGIN, DIALOG_STYLE_PASSWORD, "Account Login", "Wrong password.\nRe-enter your password to login:", "Confirm", "Cancel");
				}
			} else {
				Kick(playerid);
			}
			return Y_HOOKS_BREAK_RETURN_1;
		}
	}
	return 1;
}