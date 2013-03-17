CC=clang -g -Wall
CFLAGS=-I. -I/usr/local/include
CFLAGS_LUA=-I/usr/local/include/luajit-2.0
CFLAGS_MYSQL=-I/usr/local/include/mysql 
CFLAGS_RED=-I/usr/local/include/hiredis
LDFLAGS=-L/usr/local/lib
LDFLAGS_MYSQL=-L/usr/local/lib/mysql
LDFLAGS_LEV=-L/usr/local/lib/event2/
LIBS_LEV=-levent-2.0
LIBS_MYSQL=-lmysqlclient_r
LIBS_MEMC=-lmemcached
LIBS_LUA=-lm -lluajit-5.1 
LIBS_RED=-lhiredis 

all:
	$(CC) -c mysql.c     ${CFLAGS} ${CFLAGS_LUA} -DWITH_LUA \
		${CFLAGS_MYSQL} -DWITH_MYSQL
	$(CC) -c memcached.c ${CFLAGS} ${CFLAGS_LUA} -DWITH_MEMC \
		-DWITH_LUA
	$(CC) -c lua.c  		 ${CFLAGS} ${CFLAGS_LUA} -DWITH_LUA
	$(CC) -c redis.c	   ${CFLAGS} ${CFLAGS_LUA} -DWITH_LUA \
		${CFLAGS_RED} -DWITH_RED
	$(CC) -c http.c			 ${CFLAGS} -DWITH_HTTP \
		${CFLAGS_MYSQL} -DWITH_MYSQL \
		${CFLAGS_RED}   -DWITH_RED -DWITH_MEMC
	
	$(CC) main.c ${LDFLAGS} ${CFLAGS} \
		lua.o   -DWITH_LUA ${CFLAGS_LUA} ${LIBS_LUA} \
		cjson/fpconv.c cjson/lua_cjson.c cjson/strbuf.c -DENABLE_CJSON_GLOBAL \
		mysql.o -DWITH_MYSQL ${LDFLAGS_MYSQL} ${CFLAGS_MYSQL} ${LIBS_MYSQL} \
		redis.o -DWITH_RED ${CFLAGS_RED} ${LIBS_RED} \
		memcached.o -DWITH_MEMC ${LIBS_MEMC} \
		logger.c \
		http.o -DWITH_HTTP\
		${LDFLAGS_LEV} ${LIBS_LEV} \
		-o client

clean:
	rm -v client *.o *.core

run:
	./client
