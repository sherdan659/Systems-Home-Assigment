#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "http.h"

int main(int argc, char *argv[]) {

	int sockfd, newsockfd, num_bytes, port;
	socklen_t clilen;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in serv_addr, cli_addr;

	//Set up the TCP socket
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return 1;
	}

	port = HTTP_PORT; // port 80 used for http connections
	

	memset(&serv_addr, 0, sizeof(serv_addr)); // sets the space in the memory for the address to 0
	serv_addr.sin_family = AF_INET;  //"Address Family IPV4
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //Set the server's IP address to be the loopback address (127.0.0.1)


	/* Bind the host address */
	do {
		serv_addr.sin_port = htons(port); // alines the server port number to the server socket   and htons(port) makes sure there in the correct byte order for networking.
		fprintf(stdout, "Attempting bind() to port %d...\n", port++);
	} while (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0);

	/* Start listening for the clients (thread blocks) */
	listen(sockfd, MAX_CONNECTIONS);
	clilen = sizeof(cli_addr);

	

	return 0;
}
