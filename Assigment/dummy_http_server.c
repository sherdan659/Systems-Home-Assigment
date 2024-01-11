#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "dummy_http.h"

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, num_bytes;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in serv_addr = {0}, cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return -1;
	}

	/* allow to reuse the socket as soon as it stops being active */
	int so_reuse_enabled = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &so_reuse_enabled, sizeof(int)) < 0) {
		fprintf(stderr, "setsockopt(SO_REUSEADDR) failed");
		return -2;
	}

	/* Initialize socket structure (sockarrd_in) */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //or INADDR_ANY
	serv_addr.sin_port = htons(HTTP_PORT);

	/* Bind the host address */
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR: bind() failed\n");
		fprintf(stderr, "Error code: %s\n", strerror(errno));
		return -3;
	}

	/* Start listening for the clients (thread blocks) */
	if (listen(sockfd, MAX_CONNECTIONS) < 0) {
		fprintf(stderr, "ERROR: listen() failed\n");
		fprintf(stderr, "Error code: %s\n", strerror(errno));
		return -4;
	}

	for (;;) {
        /* Accept connection from a client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            fprintf(stderr, "ERROR: accept() failed\n");
            return -5;
        }

        /* Clear buffer and start communicating */
        memset(buffer, 0, BUFFER_SIZE);
        num_bytes = recv(newsockfd, buffer, BUFFER_SIZE-1, 0);
        if (num_bytes < 0) {
            fprintf(stderr, "ERROR: recv() failed\n");
            return -6;
        }
		time_t t1;
		time(&t1);

        printf("Received: %s\n", buffer);


        if (strcmp(buffer, "GETTIME") == 0) {
            GETTIME(newsockfd, t1);
        } 
		close(newsockfd);
	}

	close(sockfd);
	return 0;
}


void GETTIME(int client_socket, time_t t1) {

	
	time_t t2;
    time(&t2);
	char time_str_t2[100], time_str_t1[100];

	strftime(time_str_t2, sizeof(time_str_t2), "%Y-%m-%d %H:%M:%S", localtime(&t2));
	strftime(time_str_t1, sizeof(time_str_t1), "%Y-%m-%d %H:%M:%S", localtime(&t1));
    


    // Send the timestamps to the client
	send((void *)client_socket, &t2, sizeof(t2), 0);
    send((void *)client_socket, &t1, sizeof(t1), 0);


    // Optionally, you can print the timestamps on the server side
	printf("Sending time t2: %s\n", time_str_t2);
    printf("Sending time t1: %s\n", time_str_t1);

}