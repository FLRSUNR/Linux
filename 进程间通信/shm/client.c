#include "comm.h"

int main(){
    int shm_id = GetShm();
    if(shm_id < 0){
        printf("GetShm failed!\n");
        return -1;
    }
   // printf("GetShm shm_id = %d\n",shm_id);
   char *buf = (char *)attch(shm_id);
   int count = 0;
   while(1){
       buf[count++] = 'A';
       buf[count] = '\0';
       sleep(1);
   }
    return 0;
}
