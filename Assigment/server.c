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

	port = HTTP_PORT; // port 12345 
	

	memset(&serv_addr, 0, sizeof(serv_addr)); // sets the space in the memory for the address to 0
	serv_addr.sin_family = AF_INET;  //"Address Family IPV4
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //Set the server's IP address to be the loopback address (127.0.0.1)

	do {
		serv_addr.sin_port = htons(port);
		fprintf(stdout, "Attempting bind() to port %d...\n", port++);
	} while (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0);

	listen(sockfd, MAX_CONNECTIONS);
	clilen = sizeof(cli_addr);

	//Infinte loop
	for (;;) {

			newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); //waits for a connection from the client
			if (newsockfd < 0) {
				fprintf(stderr, "ERROR: accept() failed\n");
				return 3;
			}

			memset(buffer, 0, BUFFER_SIZE); //makes sure / sets the buffer to 0

			// converts the client's binary IP address to a readable string
			if (inet_ntop(AF_INET,&cli_addr.sin_addr, buffer, BUFFER_SIZE) != NULL) { 
				fprintf(stderr, "ERROR: Unable to convert client address \n");

			//printf("Client Adress: %s\n", buffer);
			//printf("Client Port:   %d\n", ntohs(cli_addr.sin_port));
			}


			memset(buffer, 0, BUFFER_SIZE);
			num_bytes = read(newsockfd, buffer, BUFFER_SIZE-1); //reads the data in newsockfd (clients data) and BUFFER_SIZE-1 makes space in the buffer for the null terminator
			if (num_bytes < 0) {
				fprintf(stderr, "ERROR: read() failed\n");
				return 4;
			}




			close(newsockfd); // closes the connection after everything is done

	}



	return 0;
}
