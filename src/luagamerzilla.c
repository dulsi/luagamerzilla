#include "luagamerzilla.h"
#include <gamerzilla.h>
#include <string.h>
#include <stdlib.h>

static int game_id = -1;
lua_State *_L = NULL;

struct fileData {
	size_t fileSize;
	char *fileContent;
	size_t fileRead;
};

size_t luagamerzilla_Size(const char *filename) {
	lua_getfield(_L, LUA_GLOBALSINDEX, "love");
	lua_getfield(_L, -1, "filesystem");
	lua_remove(_L, -2);
	lua_getfield(_L, -1, "getSize");
	lua_remove(_L, -2);
	lua_pushstring(_L, filename);
	lua_call(_L, 1, 2);
	size_t result = lua_tonumber(_L, -2);
	lua_remove(_L, -1);
	lua_remove(_L, -1);
	return result;
}

void *luagamerzilla_Open(const char *filename) {
	struct fileData *fd = (struct fileData*)malloc(sizeof(struct fileData));
	lua_getfield(_L, LUA_GLOBALSINDEX, "love");
	lua_getfield(_L, -1, "filesystem");
	lua_remove(_L, -2);
	lua_getfield(_L, -1, "read");
	lua_remove(_L, -2);
	lua_pushstring(_L, filename);
	lua_call(_L, 1, 2);
	size_t result = lua_tonumber(_L, -1);
	lua_remove(_L, -1);
	const char *data = luaL_checkstring(_L, -1);
	fd->fileSize = result;
	fd->fileContent = malloc(result);
	memcpy(fd->fileContent, data, result);
	fd->fileRead = 0;
	lua_remove(_L, -1);
	return fd;
}

size_t luagamerzilla_Read(void *f, void *buf, size_t count) {
	struct fileData *fd = (struct fileData*)f;
	if (count + fd->fileRead > fd->fileSize)
		count = fd->fileSize - fd->fileRead;
	memcpy(buf, fd->fileContent + fd->fileRead, count);
	fd->fileRead += count;
	return count;
}

void luagamerzilla_Close(void *f) {
	struct fileData *fd = f;
	free(fd->fileContent);
	free(fd);
}

EXTERN int luagamerzilla_start(lua_State *L) {
	bool server = lua_toboolean(L, 1);
	const char *arg_dir = luaL_checkstring(L, 2);
	int len = strlen(arg_dir);
	char *save_dir = malloc(len + 13);
	strcpy(save_dir, arg_dir);
	if (save_dir[len - 1] != '/') {
		strcat(save_dir, "/gamerzilla/");
	}
	bool success = GamerzillaStart(server, save_dir);
	GamerzillaSetRead(&luagamerzilla_Size, &luagamerzilla_Open, &luagamerzilla_Read, &luagamerzilla_Close);
	lua_pushboolean(L, success);
	return 1;
}

EXTERN int luagamerzilla_setGameFromFile(lua_State *L) {
	_L = L;
	const char *filename = luaL_checkstring(L, 1);
	const char *datadir = luaL_checkstring(L, 2);
	game_id = GamerzillaSetGameFromFile(filename, datadir);
	lua_pushinteger(L, game_id);
	return 1;
}

EXTERN int luagamerzilla_getTrophy(lua_State *L) {
	_L = L;
	const char *name = luaL_checkstring(L, 1);
	bool achieved = false;
	GamerzillaGetTrophy(game_id, name, &achieved);
	lua_pushboolean(L, achieved);
	return 1;
}

EXTERN int luagamerzilla_setTrophy(lua_State *L) {
	_L = L;
	const char *name = luaL_checkstring(L, 1);
	printf("%s\n", name);
	bool success = GamerzillaSetTrophy(game_id, name);
	lua_pushboolean(L, success);
	return 1;
}

EXTERN int luagamerzilla_quit(lua_State *L) {
	GamerzillaQuit();
	return 0;
}
