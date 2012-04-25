#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
int main(int argc,char **argv)
{
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;
	int sock;
	socklen_t addr_len;
	int len;
	char buff[128];

	/*  socket , SOCK_DGRAM */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(errno);
	} else
		printf("create socket.\n\r");

	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	/**/
	s_addr.sin_family = AF_INET;
	if (argv[2])
		s_addr.sin_port = htons(atoi(argv[2]));
	else
		s_addr.sin_port = htons(7838);
	if (argv[1])
		s_addr.sin_addr.s_addr = inet_addr(argv[1]);
	else
		s_addr.sin_addr.s_addr = INADDR_ANY;

	/*Bind*/
	if ((bind(sock, (struct sockaddr *) &s_addr, sizeof(s_addr))) == -1) {
		perror("bind");
		exit(errno);
	} else
		printf("bind address to socket.\n\r");

	/*while*/
	addr_len = sizeof(c_addr);
	char sendbuff[128];
	while (1) {
		len = recvfrom(sock, buff, sizeof(buff), 0,
				(struct sockaddr *) &c_addr, &addr_len);
		if (len < 0) {
			perror("recvfrom");
			exit(errno);
		}

		buff[len] = '\0';
		printf("receive from%s:%d \nmessage:%s\n\n",
				inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port), buff);
		sleep(10);
		strcpy(sendbuff,"hello");
		sendto(sock,sendbuff,strlen(sendbuff),0,
				(struct sockaddr *)&c_addr,addr_len);
		if(len<0){
			printf("\n\rsend error.\n\r");
			return 3;
		}
		printf("send:%s\n\n",sendbuff);
	}
	return 0;
}
