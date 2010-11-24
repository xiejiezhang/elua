// Interface with core services

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "auxmods.h"
#include "lrotable.h"
#include "legc.h"
#include "version.h"
#include <string.h>
#include "platform_conf.h"
#include "udl.h"

// Lua: elua.egc_setup( mode, [ memlimit ] )
static int egc_setup( lua_State *L )
{
  int mode = luaL_checkinteger( L, 1 );
  unsigned memlimit = 0;

  if( lua_gettop( L ) >= 2 )
    memlimit = ( unsigned )luaL_checkinteger( L, 2 );
  legc_set_mode( L, mode, memlimit );
  return 0;
}


// Lua: elua.version()
static int version( lua_State *L )
{
  lua_pushstring( L, ELUA_STR_VERSION );
  return 1;
}

extern u32 udl_functable[];
extern int printf_test( const char *format, ... );
static int testlookup( lua_State *L )
{
  int id = luaL_checkinteger( L, 1 );
  
  printf( "Address for symbol %d is %08X\n", id, ( unsigned int )udl_get_elua_symbol( id ) );
  printf( "Base table address is %08X\n", ( unsigned int )udl_functable ); 
  printf( "Calling printf via pointer...\n" );
  printf_test( "If this works, I'm %s ID is %d\n", "GOOOOOOD!!!!", id );
  return 0;
}

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
const LUA_REG_TYPE elua_map[] = 
{
  { LSTRKEY( "egc_setup" ), LFUNCVAL( egc_setup ) },
  { LSTRKEY( "version" ), LFUNCVAL( version ) },
  { LSTRKEY( "testlookup" ), LFUNCVAL( testlookup ) }, 
#if LUA_OPTIMIZE_MEMORY > 0
  { LSTRKEY( "EGC_NOT_ACTIVE" ), LNUMVAL( EGC_NOT_ACTIVE ) },
  { LSTRKEY( "EGC_ON_ALLOC_FAILURE" ), LNUMVAL( EGC_ON_ALLOC_FAILURE ) },
  { LSTRKEY( "EGC_ON_MEM_LIMIT" ), LNUMVAL( EGC_ON_MEM_LIMIT ) },
  { LSTRKEY( "EGC_ALWAYS" ), LNUMVAL( EGC_ALWAYS ) },
#endif
  { LNILKEY, LNILVAL }
};

LUALIB_API int luaopen_elua( lua_State *L )
{
#if LUA_OPTIMIZE_MEMORY > 0
  return 0;
#else
  luaL_register( L, AUXLIB_ELUA, elua_map );
  MOD_REG_NUMBER( L, "EGC_NOT_ACTIVE", EGC_NOT_ACTIVE );
  MOD_REG_NUMBER( L, "EGC_ON_ALLOC_FAILURE", EGC_ON_ALLOC_FAILURE );
  MOD_REG_NUMBER( L, "EGC_ON_MEM_LIMIT", EGC_ON_MEM_LIMIT );
  MOD_REG_NUMBER( L, "EGC_ALWAYS", EGC_ALWAYS );
  return 1;
#endif
}
