#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>


#define PORT 2137

void * handle_client(void * arg){
	int client_fd = *((int *)arg);
	

	int * ptr = NULL;
	
	free(client_fd);
	return (void *)ptr;
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
	server_addr.sin_port = PORT;
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
		pthread_create(&thread_id, NULL, &handle_client, (void *)client_fd);
		pthread_detach(thread_id);
	}

	return 0;
}
