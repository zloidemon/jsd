/**
 * @file http.c
 * 
 * @brief HTTP initialisation and handlers
 *
 */

#include <jsd.h>

/**
 * HTTP handler
 *
 * @param req pointer to evhttp_request 
 * @param cfg pointer to array with servers params
 *
 */
static void jsd_handler_request_cb(struct evhttp_request *, void *);

/**
 * @brief struct for request params
 */
struct jsd_serv_name {
    int         num;    /**< Number of server in emum */
    const char *name;   /**< Type of server */
    void (*init) (void **, jsd_conf *);
    int  (*info) (void **, const char *, char **);
} commands[] = {
#if ENABLE_MEMCACHED
  { JSD_MEMC,   "memcached", memc_conn,  jsd_memc_info },
#endif
#if ENABLE_MYSQL
  { JSD_MYSQL,  "mysql",     mysql_conn, jsd_mysql_info },
  { JSD_SPHINX, "sphinx",    mysql_conn, jsd_mysql_info },
#endif
#if ENABLE_REDIS
  { JSD_REDIS,  "redis",     redis_conn, jsd_redis_info }
#endif
};


#define num_of_commands (sizeof(commands)/sizeof(commands[0]))

/**
 * @brief need for binary search see example in man bsearch
 */
static int
compserv(const void *m1, const void *m2)
{
    struct jsd_serv_name *mi1 = (struct jsd_serv_name *) m1;
    struct jsd_serv_name *mi2 = (struct jsd_serv_name *) m2;
    return strcmp(mi1->name, mi2->name);
}

static void
jsd_handler_request_cb(struct evhttp_request *req, void *cfg)
{
  static void       *conn;
  static const char *param;
  static char       *result;
  const char        *raw_uri;
  struct evkeyvalq   uri;
  struct evbuffer   *evb = evbuffer_new();

  raw_uri  = evhttp_uri_get_query(evhttp_request_get_evhttp_uri(req));

  if ((raw_uri != NULL) && (evhttp_parse_query_str(raw_uri, &uri) == -1)) {
    result = NULL;
    jsd_logger(JSD_ERR, "Can not parse arguments in query.");
  }

  param = evhttp_find_header(&uri, "command");

  if ((param == NULL) || (*param == '\0')) {
    jsd_logger(JSD_ERR, "Bad request %s", evhttp_request_get_uri(req));
    evbuffer_add_printf(evb, "Empty request.");
    evhttp_send_reply(req, HTTP_BADREQUEST, "Bad request", evb);
  } else {
    struct jsd_serv_name serv_tmp, *server;
    serv_tmp.name = param;

    server = bsearch(&serv_tmp, commands, num_of_commands,
      sizeof(struct jsd_serv_name), compserv);

    if (server == NULL) {
      jsd_logger(JSD_ERR, "%s do not support", param);
      result = NULL;
    } else {
      server->init(&conn, (&((jsd_conf *)cfg)[server->num]));
      server->info(&conn, (&((jsd_conf *)cfg)[server->num])->handler, &result);
    }
  }

  if (result == NULL) {
      evbuffer_add_printf(evb, "Empty request.");
      evhttp_send_reply(req, HTTP_BADREQUEST, "Bad request", evb);
  } else {
      evbuffer_add_printf(evb, "%s", result);
      evhttp_send_reply(req, HTTP_OK, "OK", evb);
      free((void*)result);
  }

  if (param != NULL) {
     evhttp_clear_headers(&uri);
  }
  evbuffer_free(evb);
}

int
jsd_http_server(jsd_conf *cfg)
{
  struct event_base          *base;
  struct evhttp              *http;
  struct evhttp_bound_socket *handle;

  /**
   * Sorting array with servers and enums
   */
  qsort(commands, num_of_commands, sizeof(struct jsd_serv_name), compserv);

  unsigned short port = 0;
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    return 1;
  }

  base = event_base_new();

  if (base == NULL) {
    jsd_logger(JSD_ERR, "Couldn't create event_base: Exiting.");
    return 1;
  }

  http = evhttp_new(base);
  if (http == NULL) {
    jsd_logger(JSD_ERR, "Couldn't create evhttp. Exiting.");
    return 1;
  }

  evhttp_set_cb(http, cfg->handler, jsd_handler_request_cb, (void*)cfg);

  handle = evhttp_bind_socket_with_handle(http, cfg->host, cfg->port);
  if (handle == NULL) {
    jsd_logger(JSD_ERR, "Couldn't bind to port %d.", (int)port);
    return 1;
  }

  if (event_base_dispatch(base) == -1) {
    jsd_logger(JSD_ERR, "Couldn't dispatch loop.");
    return 1;
  }
  return 0;
}

