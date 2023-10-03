#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
int main(){
    int n;
    scanf("%d",&n);
    for (int i=0;i<n;i++){
        fork();
        sleep(5);
    }
}