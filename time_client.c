#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, newsockfd; /* Socket descriptors */
    socklen_t servlen;
    struct sockaddr_in cli_addr, serv_addr;
    char *ip = "127.0.0.1";
    int port = 3000;
    int i;
    char buf[100];
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Cannot create socket\n");
        exit(0);
    }
    printf("[+]TCP server socket created.\n");
    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(20000);
    servlen = sizeof(serv_addr);

    connect(sockfd, (struct sockaddr *)&serv_addr, servlen);
    printf("Connected to the server.\n");
    recv(sockfd, buf, 100, 0);
    printf("%s\n", buf);
    close(sockfd);
    printf("Disconnected from the server.\n");
    return 0;
}