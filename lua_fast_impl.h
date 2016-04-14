#ifndef _LUA_FAST_IMPL_
#define _LUA_FAST_IMPL_
#include "lua.h"
#include "luajit.h"
#include "lj_obj.h"

#if LUAJIT_VERSION_NUM == 20100

typedef GCtab Table;
#define XLUA_IS_NIL(o) tvisnil(o)
#define XLUA_IS_TRUE(o) tvistrue(o)
#define XLUA_IS_FALSE(o) tvisfalse(o)
#define XLUA_IS_BOOLEAN(o) tvisbool(o)
#define XLUA_IS_NUMBER(o) tvisnumber(o)
#define XLUA_IS_STRING(o) tvisstr(o)
#define XLUA_IS_TABLE(o) tvistab(o)
#define XLUA_IS_FUNCTION(o) tvisfunc(o)
#define XLUA_IS_USERDATA(o) tvisudata(o)
#define XLUA_IS_THREAD(o) tvisthread(o)
#define XLUA_IS_LIGHTUSERDATA(o) tvislightud(o)

#define XLUA_BOOLEAN(o) (boolV(o))
#define XLUA_NUMBER(o) (numV(o))
#define XLUA_STRING(o) (strVdata(o))
#define XLUA_STRING_LENGTH(o) (strV(o)->len)
#define XLUA_TABLE(o) (tabV(o))

#define XLUA_NODE_KEYVAL(node) ((node)->key)
#define XLUA_NODE_VAL(node) ((node)->val)

#define XLUA_INDEX_TO_ADDRESS(L, index) mp_index2addr(L, index)

TValue *mp_index2addr(lua_State *L, int idx);

#endif


typedef int (*foldfunc)(uint32_t ik, TValue *k, TValue *o, void *cargo);
int fmp_traverse(const Table *t, foldfunc fold, void *cargo);
int fmp_array_part (const Table *t, foldfunc func, void *cargo);
int fmp_hash_part (const Table *t, foldfunc func, void *cargo);

int fmp_len_if_array(const Table *t);

int fmp_fill_array(const Table *t, TValue **out);

int fmp_map_len(const Table *t);

#endif
