#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#define MESSAGE_SIZE 1024
int main(){
    int fd[2];
    //create unnamed pipe, file descriptors will be parent and child
    if (pipe(fd)==-1){
        printf("Error while creating pipe");
        return 1;
    }
    int id=fork();
    if (id==0){ //if child=subscriber=reader
        close(fd[1]);//close writing
        char message[MESSAGE_SIZE];
        read(fd[0],message,MESSAGE_SIZE);
        close(fd[0]);//close reading
        printf("%s",message);
    } else{ //if parent=publisher=writer
        close(fd[0]);//close reading
        char message[MESSAGE_SIZE];
        fscanf(stdin,"%s",message);
        write(fd[1],message,MESSAGE_SIZE);
        close(fd[1]);//close writing
    }    
}