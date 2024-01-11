build_all: http_client server

server: 
	gcc -Wall -o server dummy_http_server.c

http_client: 
	gcc -Wall -o http_client dummy_http_client.c

clean_all: 
	-rm http_client server

rebuild_all: clean_all build_all