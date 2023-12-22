#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "http.h"

int main(int argc, char *argv[]) {
	int sockfd, num_bytes, total_bytes = 0;
	int read_count = 0;
	struct sockaddr_in serv_addr;
	struct hostent *url;
	char buffer[BUFFER_SIZE] = {0};

    	
	if (argc < 2) {   // checking if the program is executed with 2 command-line arguments (./client http://example.com)
		printf("Error: Missing URL argument");
		return -1;
	}

    //AF_INET = IPv4    SOCK_STREAM = TCP   IPPROTO_TCP = TCP protocol
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return 1;
	}
	
	url = gethostbyname(argv[1]); // gets the ip of the host
	if (url == NULL) {
		fprintf(stderr, "ERROR: Host not found\n");
		return 2;
	}


	memset(&serv_addr, 0, sizeof(serv_addr)); // reserves memory for the server address
	serv_addr.sin_family = AF_INET; //sets the address to ipv4
	memcpy(&serv_addr.sin_addr, url->h_addr_list[0], url->h_length); //copys the url of the server and address structure
	serv_addr.sin_port = htons(HTTP_PORT); 

}