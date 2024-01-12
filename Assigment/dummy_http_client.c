#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "dummy_http.h"

void GETTIME(int sockfd, const char *filename);
void logToFile(const char *filename, const char *data);
void closefile(const char *filename);

struct TimeStamps {
    char time_str_t0[100];
    char time_str_t1[100];
    char time_str_t2[100];
    char time_str_t3[100];
}TimeList;

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *url;

    const char *defaultFilename = "default_log.txt";
    const char *filename;

    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = defaultFilename;
        printf("No log file specified. Using default filename: %s\n", defaultFilename);
    }

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return 1;
	}

	url = gethostbyname("127.0.0.1");
    if (url == NULL) {
        fprintf(stderr, "ERROR: Cant find host\n");
        return 2;
    }

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy(&serv_addr.sin_addr, url->h_addr_list[0], url->h_length);
	serv_addr.sin_port = htons(HTTP_PORT);

	if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR: Connection failed\n");
		return 3;
	}

	printf("Enter command GETTIME to get time:  ");
	char user_input[20];

    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 4;
    }

	if (strcmp(user_input, "GETTIME\n") == 0) {
		printf("User entered GETTIME\n");
        logToFile(filename, "User entered GETTIME");
		GETTIME(sockfd, filename);

	} else {
		printf("Error: GETTIME not entered\n");
        logToFile(filename, "Error: GETTIME not entered");
	}

    close(sockfd);
    closefile(filename);
	return 0;

}

void GETTIME(int sockfd, const char *filename) {
    int num_bytes;
    char request[] = "GETTIME";
    char buffer[BUFFER_SIZE];

    printf("Sending request: %s\n", request);

    // t0 time of request
    time_t t0;
    time(&t0);
    logToFile(filename, "Request: GETTIME");
    num_bytes = send(sockfd, request, strlen(request), 0);

    if (num_bytes < 0) {
        fprintf(stderr, "ERROR: Failed sending request\n");
        logToFile(filename, "Error: Failed sending request GETTIME");
        return;
    }

    //reciving t2, 
    time_t t1, t2;
    memset(buffer, 0, BUFFER_SIZE);

    num_bytes = recv(sockfd, &t2, sizeof(t2), 0);
    logToFile(filename, "Attempt to receive t2 from server");
    time_t t3;
    time(&t3);
    if (num_bytes < 0) {
        fprintf(stderr, "Error: Failed reading t2 from server\n");
        logToFile(filename, "Error: Failed reading t2 from server");
        return;
    }

    //reciving t1
    num_bytes = recv(sockfd, &t1, sizeof(t1), 0);
        logToFile(filename, "Attempt to receive t1 from server");
    if (num_bytes < 0) {
        fprintf(stderr, "EError: Failed reading t1 from server\n");
        logToFile(filename, "Error: Failed reading t1 from server");
        return;
    }

    strftime(TimeList.time_str_t0, sizeof(TimeList.time_str_t0), "%Y-%m-%d %H:%M:%S", localtime(&t0));
    strftime(TimeList.time_str_t1, sizeof(TimeList.time_str_t1), "%Y-%m-%d %H:%M:%S", localtime(&t1));
    strftime(TimeList.time_str_t2, sizeof(TimeList.time_str_t2), "%Y-%m-%d %H:%M:%S", localtime(&t2));
    strftime(TimeList.time_str_t3, sizeof(TimeList.time_str_t3), "%Y-%m-%d %H:%M:%S", localtime(&t3));

    // Print t0, t1, t2, t3  and save it to log
    char logMessage[150];
    printf("t0: %s\n", TimeList.time_str_t0);
    snprintf(logMessage, sizeof(logMessage), "t0 %s", TimeList.time_str_t0);
    logToFile(filename, logMessage);
    printf("t1: %s\n", TimeList.time_str_t1);
    snprintf(logMessage, sizeof(logMessage), "t1 %s", TimeList.time_str_t1);
    logToFile(filename, logMessage);
    printf("t2: %s\n", TimeList.time_str_t2);
    snprintf(logMessage, sizeof(logMessage), "t2 %s", TimeList.time_str_t2);
    logToFile(filename, logMessage);
    printf("t3: %s\n", TimeList.time_str_t3);
    snprintf(logMessage, sizeof(logMessage), "t3 %s", TimeList.time_str_t3);
    logToFile(filename, logMessage);

    int offset = ((TimeList.time_str_t1 - TimeList.time_str_t0) + (TimeList.time_str_t2 - TimeList.time_str_t3)) / 2;
    printf("Offset: %d\n", offset);

    int roundTripDelay = (TimeList.time_str_t3 - TimeList.time_str_t0) - (TimeList.time_str_t2 - TimeList.time_str_t1);
    printf("Round Trip Delay: %d\n", roundTripDelay);

}

void logToFile(const char *filename, const char *data) {

    FILE *file = fopen(filename, "a"); // "a" open to write  "w" write 
    if (file == NULL) {
        perror("Error opening log file");
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return;
    }

    fprintf(file, "%s\n", data);

    if (fclose(file) != 0) {
        perror("Error closing log file");
        fprintf(stderr, "Failed to close file: %s\n", filename);
        return;
    }
}

void closefile(const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opning file");
        return;
    }
    fclose(file);
}