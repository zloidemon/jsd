/**
 * @file main.c
 *
 * @brief Main file of server
 *
 * Contained simple runner and simple logic.
 *
 */

#include <jsd.h>
#include <unistd.h>

/**
 * @brief Example of usage message,
 */
static void usage(void);

/**
 * @brief Main start function
 * @param argv get pointers for:
 * -c config.lua
 */
int
main(int argc, char **argv)
{

  static jsd_conf cfg[JSD_END];
  char            *script;
  int              opt;
  int              config; 

  if (argc < 2) {
    script = "lua/config.lua";
    jsd_logger(JSD_INFO, "will used config %s", script);
  }

  while((opt = getopt(argc, argv, "c:")) != -1) {
    switch (opt) {
    case 'c':
      script = optarg;
      break;
    case '?':
      usage();
      return 1;
    }
  }

  jsd_logger(JSD_INFO, "Starting daemon.");

  jsd_logger(JSD_DEBUG, "Init lua machine for getting hosts configurations.");
  lua_State *LUA = jsd_lua_init(script);

  for (config = 0; config < JSD_END; ++config) {
    jsd_config_lua(LUA, cfg, config);
  }

  lua_close(LUA);
  jsd_http_server(cfg);

/**
 * FIX ME, need free all data from array cfg
  free((void*)cfg->handler);
  free((void*)cfg->host);
  free((void*)cfg->password);
  free((void*)cfg->user);
*/
  return 0;
}

static void
usage(void)
{
    fprintf(stderr, "Run with args:\n"
    "  -c lua/config.lua\n");
}

