/**
 * @file mysql.c
 *
 * @brief MySQL collector
 *
 */ 

#include <jsd.h>

void
mysql_conn(void **ret, jsd_conf *cfg)
{
  jsd_logger(JSD_INFO, "libmysqlclient, connecting %s:%d with script %s",
    cfg->host, cfg->port, cfg->handler);

  jsd_mysql *conn = mysql_init(NULL);

  if (mysql_real_connect(conn, cfg->host,
      ((jsd_db_conf *)(cfg->xattr))->user,
      ((jsd_db_conf *)(cfg->xattr))->password,
      ((jsd_db_conf *)(cfg->xattr))->database,
    cfg->port, NULL, 0) == NULL) {

    jsd_logger(JSD_ERR, mysql_error(conn));
    mysql_close(conn);
    conn = NULL;
  }
  jsd_logger(JSD_INFO, "libmysqlclient, connected %s:%d",
    cfg->host, cfg->port);
  *ret = conn;
}

int
jsd_mysql_info(void **raw, const char *script, char **result)
{
  if (*raw == NULL) {
    *result = NULL;
    return -1;
  }

  jsd_mysql *conn = *raw;
  MYSQL_RES *res;
  MYSQL_ROW row;

  lua_State *LUA = jsd_lua_init(script);

  if (mysql_query(conn, "show status")) {
    jsd_logger(JSD_ERR, "%s", mysql_error(conn));
    return -1;
  }
  res = mysql_use_result(conn);

  while ((row = mysql_fetch_row(res)) != NULL) { 
    jsd_parse_lua(LUA, row[0], row[1] ? row[1] : "0");
  }

  *result = jsd_stat_lua(LUA);
  lua_close(LUA);

  mysql_free_result(res);
  mysql_close(conn);
  return 0;
}

