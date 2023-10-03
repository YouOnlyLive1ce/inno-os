#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char *argv[]){
    int n=atoi(argv[1]);
    for (int i=0;i<n;i++){
        fork();
        sleep(5);
    }
    return 0;
}