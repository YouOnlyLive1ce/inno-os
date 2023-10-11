#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define MESSAGE_SIZE 1024

int main(int argc, char* argv[]){
    int id=atoi(argv[1]);
    char id_str[4];
    //open previously created pipe
    char directory[64]="/tmp/ex1/s";
    sprintf(id_str, "%d", id);
    strcat(directory,id_str);
    int fd_read=open(directory,O_RDONLY);
    if (fd_read==-1){
        printf("err read");
        return 1;
    }
    //read message and print to std
    char message[MESSAGE_SIZE];
    if (read(fd_read,message,MESSAGE_SIZE)==-1)
        return 2;
    printf("subscriber %d got message: %s",id, message);
    close(fd_read);
}