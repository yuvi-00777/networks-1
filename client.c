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
    char buf[15], ans[20];
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

    if(connect(sockfd, (struct sockaddr *)&serv_addr, servlen)<0){
      printf("not connected to server\n");
      exit(0);
    };
    printf("Connected to the server.\n");
    int t=1;
    while(1){
    printf("Enter the expression\n");
    
    printf("please enter ? at the end of expression\n");
    printf("Press -1 if u want to get disconnected\n");
    // scanf("%[^\n]s", buf);
    // fgets(buf, 15, stdin);
    // // strcat(buf,"\0");
    // send(sockfd, buf, strlen(buf)+1, 0);

    // bzero(buf,15);
    
    while (1)
    {
        fgets(buf, 15, stdin);
        if(buf[0]=='-' && buf[1]=='1'){
           close(sockfd);
           printf("Disconnected from the server.\n");
           return 0;
        }
        send(sockfd, buf, strlen(buf) + 1, 0);
        //printf("%s",buf);
       
        int sz = strlen(buf);
        int i = 0;
        while (i < sz)
        {
            if (buf[i] == '?')
                break;
            i++;
        }
        if(i<sz) break;
        for (int i = 0; i < 15; i++)
        {
            buf[i] = '\0';
        }
    }
    int t;
    i = 0;
    while((t=recv(sockfd, ans, 19, 0))>0){
        while(i<t){
            if(ans[i]=='?'){
                ans[i]=' ';
                break;
            }
            i++;
        }
        if(i<t) break;
        i=0;
    }
    printf("answer - %s\n", ans);
    }
    return 0;
}