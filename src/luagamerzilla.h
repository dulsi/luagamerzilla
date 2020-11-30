#include <lauxlib.h>
#include <lua.h>

#ifdef _WIN32
#    define EXTERN extern __declspec(dllexport)
#else
#    define EXTERN extern
#endif

EXTERN int luagamerzilla_start(lua_State *L);
EXTERN int luagamerzilla_setGameFromFile(lua_State *L);
EXTERN int luagamerzilla_getTrophy(lua_State *L);
EXTERN int luagamerzilla_setTrophy(lua_State *L);
EXTERN int luagamerzilla_quit(lua_State *L);
