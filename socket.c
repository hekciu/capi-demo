#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <regex.h>
#include <string.h>
#include <unistd.h>


#define BUFFER_SIZE 4096
#define PORT 2137

size_t build_http_response(char ** response) {
    char * responseStart = "HTTP/1.1 200 OK \r\n";
    char * headers =
        "Content-Type: text/html; charset=utf-8\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: keep-alive\r\n\r\n";
    char * stringOrSomething = "Hello from GNU/Linux C socket program";
    size_t length = strlen(responseStart) + strlen(headers) + strlen(stringOrSomething) + 1;
    *response = malloc(length);
    int lengthWithoutNull = snprintf(*response, length, "%s%s%s", responseStart, headers, stringOrSomething);
    return (size_t)lengthWithoutNull;    
}

void * handle_client(void * arg){
    printf("handle_client function called\n");
	int client_fd = *((int *)arg);
	
	char * buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

	ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

	if (bytes_received > 0) {
        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        int reti = regexec(&regex, buffer, 2, matches, 0);

        printf("got something\n");
        if (reti == 0) {
			char * response = NULL;
            printf("got http request\n");
            size_t responseSize = build_http_response(&response);
            send(client_fd, response, responseSize, 0);
            if (response != NULL) {
                free(response);
            }
            close(client_fd);
        }

        regfree(&regex);
	}
	
	free(buffer);
};

int main() {
	printf("starting server\n");
	struct sockaddr_in server_addr;

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd < 0) {
		perror("Could not create socket handle");
		return 1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);

	int bind_result = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	if(bind_result < 0) {
		perror("bind failed");
		return 1;
	}

	int listen_result = listen(server_fd, 10);

	if (listen_result < 0) {
		perror("listen failed");
		return 1;
	}

	for(;;) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int * client_fd = malloc(sizeof(int));

		*client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);	

		if(*client_fd < 0) {
			perror("accept failed");
			continue;
		}

		pthread_t thread_id;
		pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
		pthread_detach(thread_id);
	}

	return 0;
}
