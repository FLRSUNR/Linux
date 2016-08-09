#include "comm.h"

static int CommCreatMsgQ(int flg){
    key_t _key = ftok(_FILE_PATH_,_PROJ_ID_);
    if(_key < 0){
        perror("ftok");
        return -1;
    }
    else {
        printf("_key = %d\n",_key);
        int msg_id = msgget(_key,flg);
        if(msg_id < 0){
        perror("msgget");
        return -1;
        }
        else
          return msg_id;
    }
    return -1;          
}

int CreatMsgQ(){
    int flg = IPC_CREAT|IPC_EXCL|0644;
    return CommCreatMsgQ(flg);
}

int GetMsgQ(){
   /* key_t _key = ftok(_FILE_PATH_,_PROJ_ID_);
    if(_key < 0){
        perror("ftok");
        return -1;
    }
    int msg_id = msgget(_key,IPC_CREAT);
    if(msg_id < 0){
        perror("msgget");
        return -1;
    }
    return msg_id;*/
    int flg = IPC_CREAT;
    return CommCreatMsgQ(flg);
}
int DestoryMsgQ(int msg_id){
    if(msgctl(msg_id,IPC_RMID,NULL) == -1){
        perror("msgctl");
        return -1;
    }
    return 0;
}
int snd(int msg_id,int type,const char *msg){
    struct MsgBuf _msg;
    _msg.mtype =  type;
 
    strncpy(_msg.mtext,msg,strlen(msg)+1);

    if(msgsnd(msg_id,&_msg,sizeof(_msg.mtext),0) < 0){
        perror("msgsnd");
        return -1;
    }
    return 0;
}
int rcv(int msg_id,int type,char *out){
    struct MsgBuf _msg;
    _msg.mtype = type;
    memset(_msg.mtext,'\0',sizeof(_msg.mtext));
    if(msgrcv(msg_id,&_msg,sizeof(_msg.mtext),type,0) < 0){
        perror("msgrcv");
        return -1;
    }
    else{
        strcpy(out,_msg.mtext);
    }
    return 0;
}
