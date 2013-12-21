/**
 * @file jsd.h
 *
 * @brief Main include
 *
 * This is file contain difines of included files and functions.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libdaemond.h>

extern daemond d; /**< Structure which uses at logger and main */

/**
 * @brief Supported server
 */
typedef enum
{
  JSD_SERVER, /**< Default for itself server */
  JSD_MEMC,   /**< Memcached server */
  JSD_MYSQL,  /**< MySQL server */
  JSD_REDIS,  /**< Redis server */
  JSD_SPHINX, /**< Sphinx server */
  JSD_END     /**< Only for allocation array, don't use */
} jsd_serv_t;

/**
 * @brief Log levels
 */
typedef enum
{
  JSD_ERR,
  JSD_DEBUG,
  JSD_INFO
} jsd_log_t;

/**
 * Simple logger
 *
 * @param level number of level from jsd_log_t.
 * @param msg pointer to format printf
 *
 */
void jsd_logger(jsd_log_t, const char *, ...);

/**
 * @brief Default configuration struct
 */
typedef struct {
  int   port;     /**< port of service */
  char *handler;  /**< It's handler for http module and script for servers */
  char *host;     /**< IP address of server */
  void *xattr;
} jsd_conf;

/**
 * @brief Structure with information about databases connection
 */
typedef struct {
  char *database; /**< Need only for MySQL */
  char *password; /**< Password for auth */
  char *user;     /**< Username for auth */
} jsd_db_conf;

typedef struct {
  daemond *d;
} jsd_server_conf;

#include <config.h>

#include <signal.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
/**
 * HTTP worker
 *
 * @param cfg pointer to array with configuration of services.
 * @return 0 on success, -1 on failure
 */
int jsd_http_server(jsd_conf *);

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


/**
 * Create a new LUA machine and run file
 *
 * @param file a pointer to lua file
 * @return lua_State on success, exit on failure.
 * @see jsd_lua_init()
 */
lua_State *jsd_lua_init(const char *);

/**
 * Loading external lua_cjson library
 *
 * @see http://www.kyne.com.au/~mark/software/download/
 *
 * @param l pointer to lua stack
 * @return 1 if success
 */
int luaopen_cjson(lua_State *);
/**
 * Get configuration for server and save to memory
 *
 * @param LUA pointer to lua stack
 * @param r_cfg pointer to jsd_conf array
 * @param type enum type of servers from jsd_serv_t
 * @see jsd_config_lua()
 */
void jsd_config_lua(lua_State *, jsd_conf *, jsd_serv_t);

/**
 * Parse server output
 *
 * @param LUA pointer to lua stack
 * @param string pointer to string for parsing
 * @param number pointer to string with value of string
 */
void jsd_parse_lua(lua_State *, const char *, const char *);

/**
 * Get parsed stat from lua stack
 *
 * @param LUA pointer to lua stack
 * @return pointer to allocated memory for objec with parsed stats,
 * exit on failure.
 */
char      *jsd_stat_lua(lua_State *);

