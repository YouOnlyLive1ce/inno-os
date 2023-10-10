#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define MESSAGE_SIZE 1024

int main(){
    int n;
    fscanf(stdin, "%d",&n);
    char message[MESSAGE_SIZE];
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    fgets(message,MESSAGE_SIZE,stdin);
    pid_t parent=getpid();
    pid_t arrProccesses[n]; //i->pid
    int file_desriptors[n][2];
    // creating n proccesses - childs of parent
    int i=0;
    for (i=0; i<n;i++){
        arrProccesses[i]=fork();
        if (arrProccesses[i]==0)
            break;
    }
    if (arrProccesses[i]==0){//if child process
    // creating temporary FIFO in temp/ex1/s<i>
    char directory[64]="/tmp/ex1";
    char fifoname[8]="s";
    char i_str[2];
    sprintf(i_str, "%d", i+1);
    strcat(fifoname,i_str);//fifoname="s<i>"
    if (mkdir(directory, 0777) == -1) {
        if (errno!=EEXIST){
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }
    if (chdir(directory) == -1) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(fifoname, 0666) == -1) {
        if (errno!=EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }
    strcat(directory,"/");
    strcat(directory,fifoname);
    int fd_write=open(directory,O_WRONLY);
    if (fd_write==-1) {
        printf("err");
        return 1;
    }
    //read message from console and write to fifo
    if (write(fd_write,message,MESSAGE_SIZE)==-1)
        return 2;
    close(fd_write);
    exit(0);
    }
    //parent process waits for other processes to done work
    int status;
    pid_t child_pid;
    while ((child_pid = wait(&status)) != -1);
    return 0; 
}