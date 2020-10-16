#if defined _inc_guard_utility
    #endinput
#endif

#define _inc_guard_utility

#include <a_samp>

stock utils_randomCharacter() {
    return (random(2)) ? random('z' - 'a') + 'a' : random('Z' - 'A') + 'A';
}

#include <YSI_Coding\y_hooks>

hook OnGameModeInit() {
	print("INFO: modules/utility loaded");
	return 1;
}