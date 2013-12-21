project (jsd C CXX)
cmake_minimum_required(VERSION 2.8)

option (ENABLE_REDIS     "Enable support Redis" OFF)
option (ENABLE_MYSQL     "Enable support MySQL/Sphinx" OFF)
option (ENABLE_MEMCACHED "Enable support memcached" OFF)

set (common_sources
	${PROJECT_SOURCE_DIR}/http.c
	${PROJECT_SOURCE_DIR}/logger.c
	${PROJECT_SOURCE_DIR}/lua.c
	${PROJECT_SOURCE_DIR}/main.c
	${PROJECT_SOURCE_DIR}/cjson/fpconv.c
	${PROJECT_SOURCE_DIR}/cjson/lua_cjson.c
	${PROJECT_SOURCE_DIR}/cjson/strbuf.c
	${PROJECT_SOURCE_DIR}/libdaemond/src/libdaemond.c
)

macro (install_lua lua)
	install (FILES ${PROJECT_SOURCE_DIR}/lua/${lua}
		DESTINATION etc/jsd)
endmacro (install_lua)

add_definitions("-DENABLE_CJSON_GLOBAL")

set (common_libs m)

# Checking libevent

find_library (EVENT_LIB
		NAMES event-2.0
		PATH_SUFFIXES event2
		)
find_path    (EVENT_INCLUDE
		NAMES event2/event.h
		)

set (EVENT_FOUND false)

if (EVENT_LIB)
	set (EVENT_FOUND true)
	message (STATUS "Found libevent: ${EVENT_LIB}")
	set (common_libs ${common_libs} ${EVENT_LIB})
	include_directories (${EVENT_INCLUDE})
else (EVENT_LIB)
	message (FATAL_ERROR "Not found libevent")
endif (EVENT_LIB)

# Checking luajit

find_library (LUAJIT_LIB
		NAMES luajit-5.1)
find_path    (LUAJIT_INCLUDE
                NAMES lua.h
		PATH_SUFFIXES luajit-2.0)

set (LUAJIT_FOUND false)

if (LUAJIT_LIB)
	set (LUAJIT_FOUND true)
	message (STATUS "Found luajit: ${LUAJIT_LIB}")
	set (common_libs ${common_libs} ${LUAJIT_LIB})
	include_directories (${LUAJIT_INCLUDE})
else (LUAJIT_LIB)
	message (FATAL_ERROR "Not found luajit")
endif (LUAJIT_LIB)


include_directories (${PROJECT_SOURCE_DIR}/include)
include_directories (${PROJECT_SOURCE_DIR}/cjson)
include_directories (${PROJECT_SOURCE_DIR}/libdaemond/src)
include_directories (${PROJECT_BINARY_DIR}/include)

if (ENABLE_REDIS)
	find_library (REDIS_LIB
			NAMES hiredis)
	find_path    (REDIS_INCLUDE
			NAMES hiredis.h
			PATH_SUFFIXES hiredis)

	if (REDIS_LIB)
		message (STATUS "Found redis: ${REDIS_LIB}")
		set (common_libs ${common_libs} ${REDIS_LIB})
		include_directories (${REDIS_INCLUDE})
		set (common_sources ${common_sources}
			${PROJECT_SOURCE_DIR}/redis.c)
		install_lua (redis.lua)
	else (REDIS_LIB)
		message (STATUS "Not found redis lib, it disabled")
		set (ENABLE_REDIS false)
	endif (REDIS_LIB)
endif (ENABLE_REDIS)

if (ENABLE_MYSQL)
	find_library (MYSQL_LIB
			NAMES mysqlclient_r
			PATH_SUFFIXES mysql)
	find_path    (MYSQL_INCLUDE
			NAMES mysql.h
			PATH_SUFFIXES mysql)

	if (MYSQL_LIB)
		message (STATUS "Found mysql: ${MYSQL_LIB}")
		set (common_libs ${common_libs} ${MYSQL_LIB})
		include_directories (${MYSQL_INCLUDE})
		set (common_sources ${common_sources}
			${PROJECT_SOURCE_DIR}/mysql.c)
		install_lua (mysql.lua)
		install_lua (sphinx.lua)
	else (MYSQL_LIB)
		message (STATUS "Not found mysqlclient library, it disabled")
		set (ENABLE_MYSQL false)
	endif (MYSQL_LIB)
endif (ENABLE_MYSQL)

if (ENABLE_MEMCACHED)
	find_library (MEMCACHED_LIB
			NAMES memcached) 
	find_path    (MEMCACHED_INCLUDE
			NAMES libmemcached/memcached.h)

	if (MEMCACHED_LIB)
		message (STATUS "Found libmemcached: ${MEMCACHED_LIB}")
		set (common_libs ${common_libs} ${MEMCACHED_LIB})
		include_directories (${MEMCACHED_INCLUDE})
		set (common_sources ${common_sources}
			${PROJECT_SOURCE_DIR}/memcached.c)
		install_lua (memcached.lua)
	else (MEMCACHED_LIB)
		message (STATUS "Not found libmemcached, it disabled")
		set (ENABLE_MEMCACHED false)
	endif (MEMCACHED_LIB)
endif (ENABLE_MEMCACHED)

add_executable (jsd ${common_sources})
target_link_libraries (jsd ${common_libs})
install (TARGETS jsd DESTINATION bin)
install_lua (config.lua)

message (STATUS "JSD configured with enable options:")
message (STATUS "REDIS:"     ${ENABLE_REDIS})
message (STATUS "MYSQL:"     ${ENABLE_MYSQL})
message (STATUS "MEMCACHED:" ${ENABLE_MEMCACHED})

configure_file(
	"${PROJECT_SOURCE_DIR}/include/config.h.cmake"
	"${PROJECT_BINARY_DIR}/include/config.h"
)
