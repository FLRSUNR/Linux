#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080
int main(){

    int fd = socket(AF_INET,SOCK_DGRAM,0);
    if (fd < 0){
        perror("socket");
        return 1;
    }
    
    struct sockaddr_in ser_addr;
    bzero(&ser_addr,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(PORT);
    if(bind(fd,(struct sockaddr *)&ser_addr,sizeof(ser_addr)) < 0){
        perror("bind");
        return 2;
    }
    while(1){
        char buf[1024];
        char str[INET_ADDRSTRLEN];
        struct sockaddr_in cli_addr;
        socklen_t len = sizeof(cli_addr);
        
        memset(buf,'\0',sizeof(buf));
       int n =  recvfrom(fd,buf,sizeof(buf)-1,0,(struct sockaddr *)&cli_addr,&len);
        if( n == -1){
            perror("recvfrom");
            return 3;
        }

        printf("recvied feom %s at port %d\n",inet_ntop(AF_INET,&cli_addr.sin_addr,str,sizeof(str)),ntohs(cli_addr.sin_port));
       printf("cilent->server:%s\n",buf);

     //  memset(buf,'\0',sizeof(buf));
       //read();
 //   while(fgets(buf,sizeof(buf)-1,stdin) != NULL){
          n = sendto(fd,buf,n,0,(struct sockaddr *)&cli_addr,sizeof(cli_addr));
           if(n == -1){
           perror("sendto");
           return 4;
          }
        //}
    }

    return 0;
}
