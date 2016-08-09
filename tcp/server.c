#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <strings.h>

#define SERV_PORT 8080

void colect_child(int sig){
    while(waitpid(-1,0,WNOHANG) > 0){
        printf("colect child done\n");
    }
}

void *handler_data(void *arg){
    int sock = (int)arg;
    printf("create  new thread  new socket %d\n",sock);
    char buf[1024];
    while(1){
        ssize_t _s = read(sock,buf,sizeof(buf)-1);
        if(_s > 0 ){ //read success
            buf[_s] = '\0';
            printf("client -> server : %s",buf);

            write(sock,buf,strlen(buf)+1);
        }else if(_s == 0){ //client close
            printf("client is close \n");
            break;
        }
        else
            break;
    }
    close(sock);
    pthread_exit((void *)0);
}

int main(){
    int lfd,cfd;
    struct sockaddr_in ser_addr,cli_addr;
    socklen_t cli_len;
    char buf[1024];
    char str[INET_ADDRSTRLEN];
    int len,_s,on;

    lfd = socket(AF_INET,SOCK_STREAM,0);//创建socket
    
    if(lfd < 0){
        perror("socket");
        return 1;
    }
    on = 1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)); //设置地址可重复绑定
    
    bzero(&ser_addr,sizeof(ser_addr)); //将整个结构体清零
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);//获取本地任意IP地址:因为服务器上可能有多个网卡，每个网卡也可能绑定多个IP，这样设置可以在所有的IP上监听，直到与某个客户端建立了连接时才确定使用哪一个IP；
    ser_addr.sin_port = htons(SERV_PORT);
   
    
    if(bind(lfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr))){  //绑定port+ip
        perror("bind");
        return 2;
    }

   if( listen(lfd,128) < 0){ // 监听
    perror("listen");
    return 3;
   }

    cli_len = sizeof(cli_addr);
    while(1){
        cfd = accept(lfd,(struct sockaddr *)&cli_addr,&cli_len);
        if(cfd > 0){
            pthread_t id;
            pthread_create(&id,NULL,handler_data,(void *)cfd);
            pthread_detach(id); //进程分离
        }
    }

    //多进程处理多个客户端访问的情况
    // signal(SIGCHLD,colect_child);//回收子进程

    //    cfd  =  accept(lfd,(struct sockaddr *)&cli_addr,&cli_len);
    //    
    //    if(cfd > 0){//accept 成功
    //         pid_t new_thread = fork();
    //         if(new_thread < 0 ){ //子进程创建失败
    //            perror("fork");
    //         }
    //        else if(new_thread == 0){ //子进程创建成功
    //                 printf("fork done %d,receive from %s at port %d \n",getpid(),inet_ntop(AF_INET,&cli_addr.sin_addr,str,sizeof(str)),ntohs(cli_addr.sin_port));
    //                 buf[_s] = '\0';
    //                while(1){
    //                    _s = read(cfd,buf,sizeof(buf)-1);
    //                    if(_s > 0){ //read success
    //                        printf("client -> server %s\n",buf);
    //                        write(cfd,buf,strlen(buf));
    //                    }
    //                    else if(_s == 0){ //client done;
    //                        printf("client id done\n");
    //                        break;
    //                    }
    //                    else
    //                        break;
    //                //close(cfd);
    //                }
    //                close(cfd);
    //        }
    //        else { //父进程
    //            close(cfd);
    //         }    
    //    }
    //   }
    return 0;
}
