#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc,char *argv[]){
    if(argc != 3){
        printf("please enter like this:%s [IP] [PORT]",argv[0]);
        return 1;
    }

    int fd = socket(AF_INET,SOCK_DGRAM,0);
    if(fd < 0 ){
        perror("socket");
        return 2;
    }    

    struct sockaddr_in remote;
    bzero(&remote,sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(atoi(argv[2]));
    remote.sin_addr.s_addr = inet_addr(argv[1]);

    char buf[1024];
    memset(buf,'\0',sizeof(buf));
    while(fgets(buf,sizeof(buf)-1,stdin) != NULL){

        int n = sendto(fd,buf,strlen(buf),0,(struct sockaddr *)&remote,sizeof(remote));
        if( n == -1){
            perror("sendto");
            return 3;
        }

        n = recvfrom(fd,buf,sizeof(buf)-1,0,NULL,0);
        if(n == -1){
            perror("recvfrom");
            return 5;
        }

        printf("server->client:%s\n",buf);
    
    }
    close(fd);
    return 0;

}
