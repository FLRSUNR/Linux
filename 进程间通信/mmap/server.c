#include <sys/mman.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <error.h>  
     
int main(int argc, char **argv)  
{  
    int fd, nread, i;  
    struct stat sb;  
    char *mapped;  
    /* 打开文件 */  
    if ((fd = open(argv[1], O_RDWR)) < 0) {  
       perror("open");  
    }  
    /* 获取文件的属性 */  
    if ((fstat(fd, &sb)) == -1) {  
        perror("fstat");  
   }  
    /* 将文件映射至进程的地址空间 */  
    if ((mapped = (char *)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (void *)-1) {  
        perror("mmap");
    }  
  /* 文件已在内存, 关闭文件也可以操纵内存 */  
    close(fd);  
  /* 每隔两秒查看存储映射区是否被修改 */  
     while (1) {  
        printf("%s\n", mapped);  
        sleep(2);  
   }  
    return 0;  
}  
