#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define MAX 2048
int main(int argc, char **argv)
{
	struct sockaddr_in s_addr;
	int sock;
	int addr_len;
	int len;
	char buff[MAX];
	char receivebuff[MAX];
	/* socket , SOCK_DGRAM */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(errno);
	} else
		printf("create socket.\n\r");

	/**/
	s_addr.sin_family = AF_INET;
	if (argv[2])
		s_addr.sin_port = htons(atoi(argv[2]));
	else
		s_addr.sin_port = htons(7838);
	if (argv[1])
		s_addr.sin_addr.s_addr = inet_addr(argv[1]);
	else {
		printf("Must have a receiver！\n");
		exit(0);
	}

	/*send UDP*/
	addr_len = sizeof(s_addr);
	while(1){
		strcpy(buff, "hello i'm here");
		len = sendto(sock, buff, strlen(buff), 0,
			(struct sockaddr *) &s_addr, addr_len);
		if (len < 0) {
			printf("\n\rsend error.\n\r");
			return 3;
		}
		printf("send:%s.\n\n",buff);
		len = recvfrom(sock, receivebuff, sizeof(receivebuff) - 1, 0,
				(struct sockaddr *) &s_addr, &addr_len);
		if (len < 0) {
			perror("recvfrom");
			exit(errno);
		}

		receivebuff[len] = '\0';
		printf("receive from%s:%d \nmessage:%s\n\n",
				inet_ntoa(s_addr.sin_addr), ntohs(s_addr.sin_port), receivebuff);
		sleep(10);
	}
	return 0;
}
