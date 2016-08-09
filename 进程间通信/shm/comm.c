#include "comm.h"


static int CommCreatShm(int flg){
    key_t _key = ftok(_FILE_PATH_,_PROJ_ID_);
    if(_key < 0){
        perror("ftok");
        return -1;
    }
    printf("_key = %d\n",_key);
    int shm_id = shmget(_key,_SIZE_,flg);
    if(shm_id < 0){
        perror("shm_id");
        return -1;
    }
    return shm_id;
}

int CreatShm(){
    int flg = IPC_CREAT|IPC_EXCL|0644;
    return CommCreatShm(flg);
}

int GetShm(){
    int flg = IPC_CREAT;
    return CommCreatShm(flg);
}
int DestoryShm(int shm_id){
   if(shmctl(shm_id,IPC_RMID,NULL) == -1){
    perror("shmctl");
    return -1;
   }    
   return 0;
}
void *attch(int shm_id){
    return shmat(shm_id,NULL,0);
}
int dattch(void *addr){
    return shmdt(addr);
}
