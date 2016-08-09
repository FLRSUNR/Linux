#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define _MSG_SIZE_ 1024
#define _FILE_PATH_ "/home/flr/flr/code/main.c"
#define _PROJ_ID_ 0x55555

#define _SERVER_TYPE_ 1
#define _CLIENT_TYPE_ 2

struct MsgBuf {
    long mtype;   /*message type,must be >0*/
    char mtext[_MSG_SIZE_] ;  /*message data*/
};

int CreatMsgQ();
int GetMsgQ(); 
int DestoryMsgQ(int msg_id);
int snd(int msg_id,int type,const char *msg );
int rcv(int msg_id,int type,char *out);


