#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv) {
	if(argc!=2) {
		printf("Invalid usage, supply only an ip address\n");
		return -1;
	}
	char* ip = argv[1];
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		printf("Error creating socket\n");
		return 1;
	}
	struct sockaddr_in socket;
	socket.sin_family = AF_INET;
	int did_convert = inet_pton(AF_INET, argv[1], &socket.sin_addr);
	if(did_convert == 0) {
		printf("Error: invalid ip address\n");
		return 1;
	}
	int ports[] = { 21, 22, 23, 25, 80, 143, 443, 993 };
	int i=0;
	// TODO: make this not a hardcoded value
	for(i; i<8; i++) {
		socket.sin_port = htonl(ports[i]);
		int did_connect = connect(sockfd, (struct sockaddr *) &socket, sizeof(socket));
		printf("Result of connection to port %d: %d\n", ports[i], did_connect);
		if(did_connect != 0)
			printf("Error: %s\n", strerror(errno));
		if(did_connect == 0)
			printf("Port %d is accepting connections\n", ports[i]);
	}
	return 0;
}
