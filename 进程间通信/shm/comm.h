#pragma once

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define _SIZE_ 4096
#define _FILE_PATH_ "/home/flr/flr/code/main.c"
#define _PROJ_ID_ 0x55555

int CreatShm();
int GetShm();
int DestoryShm(int shm_id);
void *attch(int shm_id);
int dattch(void *addr);
