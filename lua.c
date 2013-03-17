/**
 * @file lua.c
 *
 * @brief Lua halpers
 *
 */

#include <stdlib.h>

#include <jsd.h>

/**
 * Get param from lua file
 *
 * @param LUA poitner to lua stack
 * @param name pointer to name of global name param in lua file
 * @param cfg pointer to address where need save param
 */
static void config_lua_get(lua_State *, const char *, void **);

/**
 *  Allocate memory for lua_object
 *  @param LUA pointer to lua stack
 *  @param idx lua object index
 *  @return ptr pointer to allocated memory
 */
static void *config_lua_alloc(lua_State *, const int);

/**
 * Get standrat volues from lua
 * 
 * @param LUA pointer to lua stack
 * @param cfg pointer to jsd_conf where need save param
 * @param server pointer to type of servers
 */
static void config_std_get(lua_State *, jsd_conf *, char *);

void
jsd_parse_lua(lua_State *LUA, const char *string, const char *number)
{
  lua_getglobal(LUA, "parse");
  lua_pushstring(LUA, string);
  lua_pushstring(LUA, number);

  if (lua_pcall(LUA, 2, 0, 0) != 0) {
    luaL_error(LUA, "error run parse(): %s", lua_tostring(LUA, -1));
  }
}

char *
jsd_stat_lua(lua_State *LUA)
{
  lua_getglobal(LUA, "stat");
  if (lua_pcall(LUA, 0, 1, 0) != 0) {
    luaL_error(LUA, "error jsd_stat_lua(): %s", lua_tostring(LUA, -1));
  }
  if (lua_type(LUA, -1) != LUA_TSTRING) {
    luaL_error(LUA, "error jsd_stat_lua(): incorrect return");
  }

  char *stat = config_lua_alloc(LUA, -1);
  strcpy(stat, (char*)lua_tostring(LUA, -1));
  lua_pop(LUA, 1);
  return stat;
}

lua_State *
jsd_lua_init(const char *file)
{
  lua_State *LUA = lua_open();
  luaL_openlibs(LUA);

  if (luaopen_cjson(LUA) != 1) {
    jsd_logger(JSD_ERR, "Couldn't load lua cjson library");
  }

  if (luaL_loadfile(LUA, file) || lua_pcall(LUA, 0, 0, 0)) {
    luaL_error(LUA, "error jsd_lua_init(): %s", lua_tostring(LUA, -1));
  }

  return LUA;
}

static void
config_std_get(lua_State *LUA, jsd_conf *cfg, char *server)
{
   lua_pushstring(LUA, server);

   if (lua_pcall(LUA, 1, 0, 0) != 0) {
     luaL_error(LUA, "error run parse(): %s", lua_tostring(LUA, -1));
   }

   config_lua_get(LUA, "host",    (void*)&(cfg->host));
   config_lua_get(LUA, "port",    (void*)&(cfg->port));
   config_lua_get(LUA, "handler", (void*)&(cfg->handler));
}

void
jsd_config_lua(lua_State *LUA, jsd_conf *r_cfg, jsd_serv_t type)
{
  static jsd_conf *cfg;
  cfg = &r_cfg[type];

  lua_getglobal(LUA, "config");
  switch(type) {
    case JSD_MYSQL: {
      config_std_get(LUA, cfg, "mysql");
      break;
    }
    case JSD_REDIS: {
      config_std_get(LUA, cfg, "redis");
      break;
    }
    case JSD_MEMC: {
      config_std_get(LUA, cfg, "memc");
      break;
    }
    case JSD_SPHINX: {
      config_std_get(LUA, cfg, "sphinx");
      break;
    }
    case JSD_SERVER:
    default: {
      config_std_get(LUA, cfg, "server");
      break;
    }
  }
}

static void *
config_lua_alloc(lua_State *LUA, const int idx) {
  size_t len  = lua_objlen(LUA, idx);
  void   *ptr = malloc(len);
  return ptr;
}

static void
config_lua_get(lua_State *LUA, const char *name, void **cfg)
{
  lua_getglobal(LUA, name);

  switch (lua_type(LUA, -1)) {
    case LUA_TNUMBER: {
      *((int *)cfg) = (int)lua_tonumber(LUA, -1);
      break;
    }
    case LUA_TSTRING: {
      *cfg = (char*)config_lua_alloc(LUA, -1);
      strcpy(*cfg, (char*)lua_tostring(LUA, -1));
      break;
    }
    default: {
      luaL_error(LUA, "config_lua_get(): unsupported type %s", lua_typename(LUA, lua_type(LUA, -1)));
    }
  }
}

