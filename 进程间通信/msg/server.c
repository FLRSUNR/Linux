#include "comm.h"

int main(){
    int msg_id = CreatMsgQ();
    char buf[_MSG_SIZE_];
    if(msg_id < 0){
        printf("CreatMsgQ faild!\n");
        return -1;
    }
    else{
        printf("msg_id = %d\n",msg_id);
        while(1){
        memset(buf,'\0',sizeof(buf));

        //sleep(10);
        if(rcv(msg_id,_CLIENT_TYPE_,buf) < 0){
            printf("rcv failed!\n");
            return -1;
        }
        else{  //success
            
            printf("client -> server:%s\n ",buf);

            printf("please enter:");
            fflush(stdout);

            read(0,buf,sizeof(buf)-1);
            snd(msg_id,_SERVER_TYPE_,buf);
            }
        }
    }
    if(DestoryMsgQ(msg_id) <  0){
        printf("DestoryMsgQ failed!\n");
        return -1;
    }
    else{
        printf("DestoryMsgQ success!\n");   
        return 0;
    }
}
