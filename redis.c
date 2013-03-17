/**
 * @file redis.c
 *
 * @brief Redis collector
 *
 */ 

#include <jsd.h>

void
redis_conn(void **ret, jsd_conf *cfg)
{
  jsd_logger(JSD_INFO, "redis, connecting %s:%d with script %s",
    cfg->host, cfg->port, cfg->handler);
  struct timeval timeout = { 1, 500000 }; // 1.5 seconds

  redisContext *conn = redisConnectWithTimeout(cfg->host, cfg->port, timeout);

  if (conn->err) {
    jsd_logger(JSD_ERR, "Connection error: %s.", conn->errstr);
    redisFree(conn);
    conn = NULL;
  }
  *ret = conn;
}

int
jsd_redis_info(void **raw, const char *script, char **result)
{

  if (*raw == NULL) {
    *result = NULL;
    return -1;
  }

  jsd_redis  *conn = *raw;
  lua_State  *LUA  = jsd_lua_init(script);
  redisReply *reply;

  int iter;
  const char *stats[] = 
    {
      "CPU",
      "MEMORY",
      "STATS",
      "CLIENTS",
      "SERVER",
      "REPLICATION",
      "PERSISTENCE"
    };
 
  for (iter = 0; iter < 7; ++iter) {
    reply = redisCommand(conn,"INFO %s", stats[iter]);
    jsd_parse_lua(LUA, reply->str, stats[iter]);
    freeReplyObject(reply);
  }

  *result = jsd_stat_lua(LUA);
  lua_close(LUA);

  redisFree(conn);
  return 0;
}
