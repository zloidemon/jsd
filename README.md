Simple JSON stats daemon(jsd).
===

THIS ISN'T PRODUCTION VERSION.

Goals of project:
* json stats from dbs via http
* similiar json output for popular dbs
* support lua scripts

Depended by:
* libmysqlclient
* hiredis
* libmemcached
* libevent2
* lua-cjson

Example outputs:
<pre>
memcached:
{
    "connections": 3198,
    "queries": 0,
    "maxed_out": 0,
    "agent_connect": 0,
    "uptime": 459238,
    "dist": {
        "dist_queries": 0,
        "dist_wait": 0,
        "dist_wall": 0,
        "dist_local": 0

    },
    "query_wall": 0,
    "agent_retry": 0,
    "ag": {
        "ag_dist1_0_query_timeouts": 0,
        "ag_dist1_0_connect_failures": 0,
        "ag_dist1_0_wrong_replies": 0,
        "ag_dist1_1_unexpected_closings": 0,
        "ag_dist1_1_connect_timeouts": 0,
        "ag_dist1_1_wrong_replies": 0,
        "ag_dist1_1_network_errors": 0,
        "ag_dist1_1_connect_failures": 0,
        "ag_dist1_0_connect_timeouts": 0,
        "ag_dist1_0_network_errors": 0,
        "ag_dist1_0_unexpected_closings": 0,
        "ag_dist1_1_query_timeouts": 0
    },
    "comm": {
        "command_flushattrs": 0,
        "command_status": 3197,
        "command_keywords": 0,
        "command_update": 0,
        "command_search": 0,
        "command_persist": 0,
        "command_excerpt": 0
    },
    "avg": {
        "avg_dist_local": 0,
        "avg_dist_wait": 0,
        "avg_dist_wall": 0,
        "avg_query_wall": 0
    }
}

redis:

{
    "server": {
        "lru_clock": 44453,
        "redis_git_dirty": 0,
        "uptime_in_days": 3,
        "redis_version": 2,
        "gcc_version": 4,
        "process_id": 37603,
        "hz": 10,
        "tcp_port": 6379,
        "redis_git_sha1": 0,
        "uptime_in_seconds": 297040,
        "arch_bits": 64
    },
    "repl": {
        "connected_slaves": 0,
        "role": "master"
    },
    "clients": {
        "client_biggest_input_buf": 0,
        "client_longest_output_list": 0,
        "blocked_clients": 0,
        "connected_clients": 1
    },
    "cpu": {
        "used_cpu_sys_children": 0,
        "used_cpu_user_children": 0,
        "used_cpu_sys": 22,
        "used_cpu_user": 26
    },
    "mem": {
        "mem_allocator": "libc",
        "mem_fragmentation_ratio": 1,
        "used_memory_lua": 25600,
        "used_memory_human": 1,
        "used_memory_peak": 1139248,
        "used_memory_peak_human": 1,
        "used_memory": 1139368,
        "used_memory_rss": 1139368
    },
    "pers": {
        "aof_rewrite_scheduled": 0,
        "aof_rewrite_in_progress": 0,
        "rdb_last_save_time": 1363296296,
        "aof_enabled": 0,
        "loading": 0,
        "rdb_bgsave_in_progress": 0,
        "rdb_changes_since_last_save": 0
    },
    "stats": {
        "evicted_keys": 0,
        "keyspace_hits": 0,
        "instantaneous_ops_per_sec": 0,
        "keyspace_misses": 0,
        "pubsub_channels": 0,
        "latest_fork_usec": 0,
        "pubsub_patterns": 0,
        "rejected_connections": 0,
        "total_commands_processed": 535010,
        "total_connections_received": 76432,
        "expired_keys": 0
    }
}
</pre>
