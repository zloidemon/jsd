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
 * -C commands: /start/stop/check/
 * -p pid: /path/to/file.pid
 * -v verbose run
 */

daemond d; /**< External damond struct */

int
main(int argc, char **argv)
{

  static jsd_conf cfg[JSD_END];
  char            *script  = NULL,
                  *command = NULL,
                  *pidfile = NULL;
  int              opt;
  int              config; 

  daemond_init(&d);

  d.use_pid     = 1;
  d.detach      = 1;
  d.pid.verbose = 1;

  if (argc < 2) goto help;

  while((opt = getopt(argc, argv, "c:C:p:vh")) != -1) {
    switch (opt) {
    case 'c':
      script  = optarg;
      break;
    case 'C':
      command = optarg;
      break;
    case 'p':
      pidfile = optarg;
      break;
    case 'v':
      d.detach = 0;
      break;
    case 'h':
      goto help;
      break;
    }
  }

  if (!command) goto help;
  if (!script)  goto help;

  lua_State *LUA = jsd_lua_init(script);

  if (pidfile)
    d.pid.pidfile = pidfile;

  cfg[JSD_SERVER].xattr = malloc(sizeof(jsd_server_conf));
  cfg[JSD_MYSQL].xattr  = malloc(sizeof(jsd_db_conf));

  ((jsd_server_conf *)cfg[JSD_SERVER].xattr)->d = &d;

  for (config = 0; config < JSD_END; ++config) {
    jsd_config_lua(LUA, cfg, config);
  }

  lua_close(LUA);

  /* Demonize */
  daemond_cli_run(&d.cli, 1, &command);
  daemond_say(&d, "<g>Starting</>... (pidfile = %s, pid = <y>%d</>)",
                 d.pid.pidfile, getpid());
  daemond_daemonize(&d);	
  daemond_master(&d);
  daemond_say(&d, "Forked: %d",getpid());

  jsd_http_server(cfg);

/**
 * FIX ME, need free all data from array cfg
  free((void*)cfg->handler);
  free((void*)cfg->host);
  free((void*)cfg->password);
  free((void*)cfg->user);
*/
  return 0;

help:
  usage();
  return 1;

}

static void
usage(void)
{
    fprintf(stderr, "Run with args:\n"
    "  -c lua/config.lua\n"
    "  -C start/stop/check\n"
    "  -p /path/to/file.pid\n"
    "  -v (verbose running)\n"
    );
}

