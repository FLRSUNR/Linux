#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>

void usage(const char *proc){
    printf("%s [IP] [PORT]",proc);
}
int main(int argc,char *argv[]){

    if(argc != 3){
        usage(argv[0]);
        return 3;
    }
    size_t i = 0;
    for( i = 0;i < argc ; ++i){
        printf("%s\n",argv[i]);
    }
    int cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd < 0){
        perror("socket");
        return 1;
    }

    struct sockaddr_in remote;
    bzero(&remote,sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(argv[1]);
    remote.sin_port = htons(atoi(argv[2]));
    
    if(connect(cfd,(struct sockaddr *)&remote,sizeof(remote)) < 0){
        perror("connect");
        return 2;
    }
    char buf[1024];
    while(1){
        ssize_t _s = read(0,buf,sizeof(buf)-1);

        if(_s > 0){ //read success
            buf[_s] = '\0';
            write(cfd,buf,strlen(buf));

            //_s = read(cfd,buf,sizeof(buf)-1);
            read(cfd,buf,sizeof(buf)-1);
            //buf[_s] = '\0';
            printf("server -> cilent : %s\n",buf);
        }
    }

    return 0;
}
