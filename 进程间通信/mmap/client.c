#include <stdio.h>
#include <error.h>
#include <sys/mman.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <error.h> 

  
int main(int argc, char **argv)  
{  
        int fd, nread, i =  0;  
         struct stat sb;  
         char *mapped;           
         /* 打开文件 */  
          if ((fd = open(argv[1], O_RDWR,0644)) < 0) {  
                 perror("open");  
            }  
                          
         /* 获取文件的属性 */  
          if ((fstat(fd, &sb)) == -1) {  
             perror("fstat");  
          }  
                              
         /* 共享文件映射将无法修改文件 */  
         if ((mapped = (char *)mmap(NULL,1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == (void *)-1) {  
             perror("mmap");  
          }  
                                 
         /* 映射完后, 关闭文件也可以操纵内存 */  
        close(fd);  
                                      
         /* 修改一个字符 */
        while(1){
            mapped[i++] = '#';  
            mapped[i] = '\0';
            sleep(1);
        }
      //  munmap(mapped,sb.st_size);
        return 0;  
}                   
