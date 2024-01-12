build_all: http_client server

http_client: 
	gcc -Wall -o http_client dummy_http_client.c

server: 
	gcc -Wall -o server dummy_http_server.c

clean_all: 
	-rm http_client server

rebuild_all: clean_all build_all

debug: 
	mkdir -p bin/dbg
	gcc -Wall -g -o bin/dbg/http_client dummy_http_client.c
	gcc -Wall -g -o bin/dbg/server dummy_http_server.c

release:
	mkdir -p bin/rel
	gcc -Wall -O2 -o bin/rel/http_client dummy_http_client.c
	gcc -Wall -O2 -o bin/rel/server dummy_http_server.c