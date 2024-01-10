#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "dummy_http.h"

int main(int argc, char *argv[]) {
	int sockfd, num_bytes;
	struct sockaddr_in serv_addr;
	struct hostent *url;
	char buffer[BUFFER_SIZE] = {0};

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return 1;
	}

	url = gethostbyname("127.0.0.1"); // Use the server's IP address
    if (url == NULL) {
        fprintf(stderr, "ERROR: Host not found\n");
        return 2;
    }

	/* Initialize socket structure (sockarrd_in) */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy(&serv_addr.sin_addr, url->h_addr_list[0], url->h_length);
	serv_addr.sin_port = htons(HTTP_PORT);

	/* Connect to the server */
	if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR: Connection failed\n");
		return 3;
	}



	printf("Enter command GETTIME to get time:  ");
	char user_input[20];

	fgets(user_input, sizeof(user_input), stdin);

	if (strcmp(user_input, "GETTIME\n") == 0) {
		printf("User entered GETTIME\n");
		GETTIME(sockfd);
	} else {
		printf("Error: GETTIME not entered\n");
	}

	return 0;

}

int GETTIME(int sockfd) {
    int num_bytes;
    char request[] = "GETTIME";
    char buffer[BUFFER_SIZE];

    printf("Sending request: %s\n", request);

    // Send the GETTIME request
    num_bytes = send(sockfd, request, strlen(request), 0);
    if (num_bytes < 0) {
        fprintf(stderr, "ERROR: Failed writing to socket\n");
        return -1;
    }

    // Receive and print the server's response
    do {
        memset(buffer, 0, BUFFER_SIZE);
        num_bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (num_bytes > 0) {
            // Check if the received data contains "Current time:"
            char *time_ptr = strstr(buffer, "Current time:");
            if (time_ptr != NULL) {
                printf("Received from server: %s \n", time_ptr);
                // Process the time information as needed
            }
        }
    } while (num_bytes > 0);

    if (num_bytes < 0) {
        fprintf(stderr, "ERROR: Failed reading from socket\n");
        return -2;
    }

    return 0;
}