#include "luagamerzilla.h"

void add_func(lua_State *L, const char *name, lua_CFunction func) {
	lua_pushcfunction(L, func);
	lua_setfield(L, -2, name);
}

// Creates and returns a table with all functions
EXTERN int luaopen_luagamerzilla(lua_State *L) {
	lua_createtable(L, 0, 5);
	add_func(L, "start", luagamerzilla_start);
	add_func(L, "setGameFromFile", luagamerzilla_setGameFromFile);
	add_func(L, "getTrophy", luagamerzilla_getTrophy);
	add_func(L, "setTrophy", luagamerzilla_setTrophy);
	add_func(L, "quit", luagamerzilla_quit);
	return 1;
}
