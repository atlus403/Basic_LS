#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE (128 * 1024)

struct linux_dirent64 {
    unsigned long long d_ino;
    long long          d_off;
    unsigned short     d_reclen;
    unsigned char      d_type;
    char               d_name[];
};

int main(int argc, char **argv){

    int fd = open(argv[1], O_RDONLY);

    char *malloc_buf = malloc(BUFFER_SIZE); // 128 KB [Arbitrary]
    if(malloc_buf == NULL)
    {
        return 1;
    } 

    long read_length = 0;

    // -1 [Error] == while(-1) == True
    while(((read_length = syscall(SYS_getdents64,fd,malloc_buf,BUFFER_SIZE)) > 0)){
        for(int offset = 0; offset < read_length;){
            struct linux_dirent64 *d = (struct linux_dirent64*)(malloc_buf + offset);
            
            offset += d->d_reclen;
            if (d->d_name[0] == '.') continue; // Hiden . .. and hidden files
            printf("%s\t", (d->d_name));
        }
    }

    close(fd);
    free(malloc_buf);

    return 0;
}