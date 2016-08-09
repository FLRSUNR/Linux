#include "comm.h"

int main(){
    int msg_id = GetMsgQ();
    char buf[_MSG_SIZE_];
    if(msg_id < 0){
        printf("GettMsgQ faild!\n");
        return -1;
    }
    else{ //Get success
        printf("GetMsgQ msg_id = %d\n",msg_id);
        while(1){
        memset(buf,'\0',sizeof(buf));

        read(0,buf,sizeof(buf)-1);
       // sleep(5);
        if(snd(msg_id,_CLIENT_TYPE_,buf) < 0){
            printf("snd message failed!\n");
            return -1;
        }
        else{  //snd success
            rcv(msg_id,_SERVER_TYPE_,buf);
            printf("server -> client:%s\n ",buf);

            printf("please enter:");
            fflush(stdout);
            }
        }
    }
    
    return 0;
}
