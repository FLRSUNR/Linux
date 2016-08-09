#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

void Usage(const char *proc){
    printf("please enter like :%s [IP] {PORT}",proc);
}


int StartSock(const char *ip,const int port){
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0){
        perror("socket");
        return 2;
    }

    int on = 1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    struct sockaddr_in local;

    bzero(&local,sizeof(local));
    local.sin_family =  AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ip); // htonl(INADDR_ANY);

    if(bind(fd,(struct sockaddr *)&local,sizeof(local)) < 0){
        perror("bind");
        return 3;
    }

    int ret = listen(fd,5);
    if(ret < 0){
        perror("listen");
        return 4;
    }
    printf("listen socket success\n");
    return fd;
}

int main(int argc,char *argv[]){
    if(argc != 3){
        Usage(argv[0]);
        return 1;
    }

    int listen_socket = StartSock(argv[1],atoi(argv[2]));

    int new_fd;
    char fds[128];
    const int len = 5;

   // memset(fds,-1,sizeof(fds));
   //初始化fds[]为-1
   for(size_t i = 0; i < len ;++i)
        fds[i] = -1;
   printf("init fds done \n"); 
   fd_set rfds;
   fds[0] = listen_socket;

   int max_fd = -1;
   while(1){
    max_fd = -1;
    FD_ZERO(&rfds); //清空rfds;
    
    for(int i = 0; i < len ; ++i){ //添加文件描述符给rfds，并且更新最大文件描述符max_fd;
        if(fds[i] !=  -1){
             FD_SET(fds[i],&rfds);
             if(max_fd < fds[i])
                 max_fd = fds[i];
        }
    }

    for(size_t i = 0;i<len;++i){
        printf("%d ",fds[i]);
    }
    printf("fds[] \n**************************************\n");

    
    printf("max_fd = %d\n***********************************\n",max_fd);
   int res = select(max_fd+1,&rfds,NULL,NULL,NULL);
    switch(res ){
        case 0:
            printf("timeout\n");
            break;
        case -1:
            perror("select");
            exit(1);
            break;
        default:{
                    int index = 0;
                    for(;index < len;++index){
                        if(index == 0 && fds[index] != -1 && FD_ISSET(fds[index],&rfds)){ //fds[0] is listen_socket
                            //new accept
                            struct sockaddr_in peer;
                            socklen_t perlen = sizeof(peer);
                            new_fd = accept(listen_socket,(struct sockaddr*)&peer,&perlen);
                            if(new_fd != -1){
                                printf("++++++++++++++++++++++++++++\nget new accept,%d\n++++++++++++++++++++++++++++\n",new_fd);
                               int i = 0;
                                for(i = 0; i < len; ++i){
                                    if(fds[i] == -1){
                                        fds[i] = new_fd;
                                        break;
                                    }
                                }
                                if(i == len){ //fds[] full
                                    printf("fds full, close new fd,%d\n",new_fd);
                                    close(new_fd);
                                }                           
                            }
                            continue;
                        }

                        if(fds[index] != -1 && FD_ISSET(fds[index],&rfds)){//just for read
                            char buf[1024];
                            memset(buf,'\0',sizeof(buf));

                          
                            // ssize_t size = recv(fds[index],buf,sizeof(buf)-1,0);//read data
                            ssize_t size = read(fds[index],buf,sizeof(buf)-1);
                            if(size == 0){//client done
                                printf("remote client done,size is %d\n",fds[index]);
                                close(fds[index]);
                                FD_CLR(fds[index],&rfds);
                                fds[index] = -1;

                            }else{
                                printf("client%d:%s",fds[index],buf);
                                
                                fgets(buf,sizeof(buf),stdin);
                                write(fds[index],buf,strlen(buf));
                            }
                        }
                    }

                    }
                
               break;
    }
   }
 return 0;
}
