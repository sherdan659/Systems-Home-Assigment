build_all: clean http_client server

server: 
	gcc -Wall -o server dummy_http_server.c

http_client: 
	gcc -Wall -o http_client dummy_http_client.c

clean: 
	-rm http_client server

rebuild_all: clean build_all