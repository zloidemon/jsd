/**
 * @file memcached.c
 *
 * @brief Memcached collector
 *
 */ 

#include <jsd.h>

static memcached_return_t
stat_printer(memcached_server_instance_st instance,
                                       const char *key, size_t key_length,
                                       const char *value, size_t value_length,
                                       void *context)
{
  static memcached_server_instance_st last= NULL;
  
  if (last != instance)
  {
    jsd_logger(JSD_INFO, "memcached, connecting %s:%u",  memcached_server_name(instance),
           (uint32_t)memcached_server_port(instance));
    last = instance;
  }

    jsd_parse_lua((lua_State*)context, key, value);

  return MEMCACHED_SUCCESS;
}

void
memc_conn(void **ret, jsd_conf *cfg)
{
  memcached_return rc;

  memcached_st        *memc    = memcached_create(NULL);
  memcached_server_st *servers = NULL;
  servers = memcached_server_list_append(servers, cfg->host, cfg->port, &rc);
  rc      = memcached_server_push(memc, servers);
  *ret = memc;
}

int
jsd_memc_info(void **raw, const char *script, char **result)
{
  if (*raw == NULL) {
    *result = NULL;
    return -1;
  }

  jsd_memc  *memc = *raw;
  lua_State *LUA  = jsd_lua_init(script);

  memcached_return_t status = memcached_stat_execute(memc, NULL, stat_printer, (void*)LUA);
  if (status != MEMCACHED_SUCCESS) {
    jsd_logger(JSD_ERR, memcached_strerror(memc, status));
    lua_close(LUA);
    return -1;
  }

  *result = jsd_stat_lua(LUA);
  lua_close(LUA);

  memcached_free(memc);
  return 0;
}
