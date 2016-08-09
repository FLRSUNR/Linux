#include "comm.h"

int main(){
    int shm_id = CreatShm();
    if(shm_id < 0){
        printf("CreatShm failed!\n");
        return -1;
    }

    //printf("CreatShm shm_id = %d\n",shm_id);
    char *buf = (char *)attch(shm_id);
    while(1){
        printf("%s\n",buf);
        sleep(1);
    }
    dattch(buf);
    DestoryShm(shm_id);
    return 0;
}
