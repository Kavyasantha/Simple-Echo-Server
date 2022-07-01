#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>
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
int main(int args, char **argv)
{
    int sock;
    int new;
    
    struct sockaddr_in server;
    struct sockaddr_in client;
    
    int sockaddr_len=sizeof(struct sockaddr_in);
    char data[1024];
   
    

    if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("--- error in connection \n");
        exit(1);
    }
    server.sin_family=AF_INET;
    server.sin_port=htons(atoi(argv[1]));
    server.sin_addr.s_addr=INADDR_ANY;
    bzero(&server.sin_zero,8);

    if((bind(sock,(struct sockaddr*)&server,sockaddr_len))==-1)
    {
        perror("--- error in binding \n");
        exit(1);
    }
    if((listen(sock,4))==0)
    printf("Server : listening in progress\n");
    else 
    {
        perror("--- error in listening \n");
        exit(1);
    }
    pid_t childpid;
    
    while(1)
    {
        new=accept(sock,(struct sockaddr*)&client,&sockaddr_len);
        if(new==-1)
        {
            perror("--- error in accepting \n");
            continue;
        }
        
        printf("Server : connection accepted\n");
        printf("Server : Connection from port number %d and IP %s\n",ntohs(client.sin_port),inet_ntoa(client.sin_addr));
        
        if((childpid=fork())==0)
        { 
            close(sock);
            ssize_t len;
            if(childpid==0)
    {
        printf(" child process  ID is   =  %d\n" , getpid());
    }
            again:
            while((len=read(new,data,1024))>0)
            {
                
                writen(new, data,len);
                printf("echoed msg from server : %s",data);
            }   
                
            if(len<0 && errno==EINTR)
                goto again;
            else if (len<0)
                {
                perror("read error\n");
                exit(1);
                }
        exit(0);
        }
        
        close(new);  
        
    }          
    printf("client disconnected\n");
    close(sock);
    return 0;
}   