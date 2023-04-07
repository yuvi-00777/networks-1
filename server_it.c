/*
            NETWORK PROGRAMMING WITH SOCKETS

In this program we illustrate the use of Berkeley sockets for interprocess
communication across the network. We show the communication between a server
process and a client process.


*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE 100

/* declared here as global variable because stack[]
 * is used by more than one fucntions */
float numbers[1000];
int tn = -1, to = -1;
char op[1000];

void push_num(float n)
{
    numbers[++tn] = n;
}
void push_op(char ch)
{
    op[++to] = ch;
}
float pop_num()
{
    return numbers[tn--];
}
char pop_op()
{
    return op[to--];
}

float infix_evalution(float numbers[1000], char op[1000])
{

    char p;
    float x = pop_num();
    float y = pop_num();
    p = pop_op();
    switch (p)
    {
    case '+':
        return x + y;
    case '-':
        return y - x;
    case '*':
        return x * y;
    case '/':
        if (x == 0)
        {
            printf("divide by zero");
            exit(0);
        }
        else
        {
            return y / x;
        }
    }
}

int is_op(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int precedence(char c)
{
    switch (c)
    {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 1;
    case '/':
        return 1;
    }
    return -1;
}
int isDIGIT(char c)
{
    return ((c >= '0' && c <= '9') || c == '.');
}
float valuation(char exp[100])
{
    float num, dec, v, output;
    char c;
    for (int i = 0; exp[i] != '\0' && exp[i] != '?'; i++)
    {
        c = exp[i];
        if (exp[i] == ' ' || exp[i] == '\n')
        {
            continue;
        }
        if (isDIGIT(c))
        {
            num = 0;
            dec = 0;
            while (isDIGIT(c))
            {
                if (c == '.')
                {
                    dec = 10.0;
                }
                else
                {
                    if (dec > 0)
                    {
                        num += (c - '0') / dec;
                        dec = dec * 10;
                    }
                    else
                    {
                        num *= 10;
                        num += c - '0';
                    }
                }
                i++;
                if (i < strlen(exp))
                {
                    c = exp[i];
                }
                else
                {
                    break;
                }
            }
            i--;
            push_num(num);
        }
        else if (c == '(')
        {
            push_op(c);
        }
        else if (c == ')')
        {
            while (op[to] != '(')
            {
                v = infix_evalution(numbers, op);
                push_num(v);
            }
            pop_op();
        }
        else if (is_op(c))
        {
            while (to != -1 && precedence(c) <= precedence(op[to]))
            {
                output = infix_evalution(numbers, op);
                push_num(output);
            }
            push_op(c);
        }
    }
    while (to != -1)
    {
        output = infix_evalution(numbers, op);
        push_num(output);
    }
    return pop_num();
}
int main()
{
    int sockfd, newsockfd; /* Socket descriptors */
    socklen_t clilen;
    char *ip = "127.0.0.1";
    int i, op1, op2, res, x;
    char ch;

    struct sockaddr_in cli_addr, serv_addr;
    int port = 3000;

    char buf[1000], ans[30]; /* We will use this buffer for communication */

    /* The following system call opens a socket. The first parameter
       indicates the family of the protocol to be followed. For internet
       protocols we use AF_INET. For TCP sockets the second parameter
       is SOCK_STREAM. The third parameter is set to 0 for user
       applications.
    */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Cannot create socket\n");
        exit(0);
    }
    //    memset(&serv_addr, '\0', sizeof(serv_addr));
    /* The structure "sockaddr_in" is defined in <netinet/in.h> for the
       internet family of protocols. This has three main fields. The
       field "sin_family" specifies the family and is therefore AF_INET
       for the internet family. The field "sin_addr" specifies the
       internet address of the server. This field is set to INADDR_ANY
       for machines having a single IP address. The field "sin_port"
       specifies the port number of the server.
    */

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

    /* With the information provided in serv_addr, we associate the server
       with its port using the bind() system call.
    */
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("Unable to bind local address\n");
        exit(0);
    }
    listen(sockfd, 5); /* This specifies that up to 5 concurrent client
                  requests will be queued up while the system is
                  executing the "accept" system call below.
               */
    printf("listening\n");
    /* In this program we are illustrating an iterative server -- one
       which handles client connections one by one.i.e., no concurrency.
       The accept() system call returns a new socket descriptor
       which is used for communication with the server. After the
       communication is over, the process comes back to wait again on
       the original socket descriptor.
    */
    while (1)
    {

        /* The accept() system call accepts a client connection.
           It blocks the server until a client request comes.

           The accept() system call fills up the client's details
           in a struct sockaddr which is passed as a parameter.
           The length of the structure is noted in clilen. Note
           that the new socket descriptor returned by the accept()
           system call is stored in "newsockfd".
        */
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);
        printf("[+]Client connected.\n");
        if (newsockfd < 0)
        {
            perror("Accept error\n");
            exit(0);
        }
        while (1)
        {
            for (int i = 0; i < 1000; i++)
            {
                buf[i] = '\0';
            }
            for (int i = 0; i < 20; i++)
            {
                ans[i] = '\0';
            }
            int t;
            i = 0;
            while ((t = recv(newsockfd, ans, 5, 0)) > 0)
            {
                i = 0;
                // printf("%s\n", ans);
                strcat(buf, ans);
                while (i < t)
                {
                    if (ans[i] == '?')
                        break;
                    i++;
                }
                if (i < t)
                    break;
                for (int i = 0; i < 20; i++)
                {
                    ans[i] = '\0';
                }
            }
            if (t == 0)
                break;
            i = 0;
            // recv(newsockfd, ans, 20, 0);
            // strcat(buf, ans);
            // send(newsockfd, buf, strlen(buf), 0);
            /* We now receive a message from the client. For this example
               we make an assumption that the entire message sent from the
               client will come together. In general, this need not be true
               for TCP sockets (unlike UDPi sockets), and this program may not
               always work (for this example, the chance is very low as the
               message is very short. But in general, there has to be some
               mechanism for the receiving side to know when the entire message
              is received. Look up the return value of recv() to see how you
              can do this.
            */
            // send(newsockfd, ans, 100, 0);
            float f = valuation(buf);
            // printf("%f", f);
            strcpy(ans, gcvt(f, 6, buf));
            strcat(ans, "?");
            send(newsockfd, ans, strlen(ans) + 1, 0);
        }
        close(newsockfd);
        printf("client disconnected\n");
    }
    return 0;
}
