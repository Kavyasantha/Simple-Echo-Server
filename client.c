#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>

#define buffer 1024 

ssize_t writen(int sock, char *data, int n) 
{
size_t nleft;
ssize_t nwritten;
const char *ptr;
ptr = data;
nleft = n;
while (nleft > 0) {
    if ( (nwritten = write(sock, data, nleft)) <= 0) {
        if (nwritten < 0 && errno == EINTR)
            nwritten=0;
        else 
            return(-1);
    }
    nleft -= nwritten;
    ptr += nwritten;
    }
    return (n);
}
ssize_t readline(int sock, void *data, size_t maxlen)
{
    ssize_t n, rc;
    char    c, *ptr;
    ptr = data;
    for (n = 1; n < maxlen; n++) {
        again:
            if ((rc = read(sock, &c,1) == 1)) {
                *ptr++ = c;
                if (c == '\n') 
                    break;
            } else if (rc == 0) {
                *ptr = 0;
                return (n - 1);
            } else {
                if (errno == EINTR)
                    goto again;
                    return (-1);
            }
    }
    *ptr = 0;
    return (n);
}



int main(int argc, char **argv)
{
    struct sockaddr_in remote_server;
    int sock;
    char input[buffer];
    char output[buffer];
    int sockaddr_len=sizeof(struct sockaddr_in);
    int n;
    

    sock = socket(AF_INET,SOCK_STREAM,0);
    remote_server.sin_family=AF_INET;
    remote_server.sin_port=htons(atoi(argv[2]));
    remote_server.sin_addr.s_addr=inet_addr(argv[1]);
    bzero(&remote_server.sin_zero, 8);

    connect(sock,(struct sockaddr*)&remote_server,sockaddr_len);

    while(fgets(input,buffer,stdin)!=NULL)
    {
        writen(sock,input,strlen(input));
        
        if((n=readline(sock,output,buffer)==0))
        {
        perror("server terminated");
        exit(1);
        }
        
        fputs(output,stdout);
    }
    exit(0);
    return 0;
}

