Simple JSON stats daemon(jsd).
===

THIS ISN'T PRODUCTION VERSION.

Gools of project:
* json stats from dbs via http
* similiar json output for popular dbs
* support lua scripts

Depended by:
* libmysqlclient
* hiredis
* libmemcached
* libevent2
* lua-cjson

Example output:
<pre>
[jsd] 01:10 %> curl 'http://localhost:3333/admin.jsp?command=memcached'
{"incr_misses":0,"auth_cmds":0,"auth_errors":0,"bytes":0,"rusage_system":5.394793,"connection_structures":6,"total_connections":267,"time":1363554636,"pointer_size":64,"touch_misses":0,"hash_is_expanding":0,"curr_connections":5,"cmd_flush":0,"cmd_touch":0,"limit_maxbytes":67108864,"cas_misses":0,"cas_badval":0,"incr_hits":0,"bytes_read":6008,"cas_hits":0,"hash_bytes":524288,"cmd_set":0,"get_hits":0,"rusage_user":5.794673,"bytes_written":274094,"listen_disabled_num":0,"reclaimed":0,"evictions":0,"evicted_unfetched":0,"conn_yields":0,"accepting_conns":1,"curr_items":0,"decr_misses":0,"total_items":0,"pid":1477,"touch_hits":0,"expired_unfetched":0,"threads":4,"delete_misses":0,"hash_power_level":16,"uptime":1163623,"delete_hits":0,"cmd_get":0,"decr_hits":0,"reserved_fds":20,"get_misses":0}

[jsd] 01:11 %> curl 'http://localhost:3333/admin.jsp?command=sphinx'
{"connections":3198,"queries":0,"maxed_out":0,"agent_connect":0,"uptime":459238,"dist":{"dist_queries":0,"dist_wait":0,"dist_wall":0,"dist_local":0},"query_wall":0,"agent_retry":0,"ag":{"ag_dist1_0_query_timeouts":0,"ag_dist1_0_connect_failures":0,"ag_dist1_0_wrong_replies":0,"ag_dist1_1_unexpected_closings":0,"ag_dist1_1_connect_timeouts":0,"ag_dist1_1_wrong_replies":0,"ag_dist1_1_network_errors":0,"ag_dist1_1_connect_failures":0,"ag_dist1_0_connect_timeouts":0,"ag_dist1_0_network_errors":0,"ag_dist1_0_unexpected_closings":0,"ag_dist1_1_query_timeouts":0},"comm":{"command_flushattrs":0,"command_status":3197,"command_keywords":0,"command_update":0,"command_search":0,"command_persist":0,"command_excerpt":0},"avg":{"avg_dist_local":0,"avg_dist_wait":0,"avg_dist_wall":0,"avg_query_wall":0}}
</pre>
