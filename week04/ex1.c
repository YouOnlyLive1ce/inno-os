#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include <time.h>

int main(){
pid_t parent = getpid();

pid_t child1 =fork();
if (getpid()==parent){
    pid_t child2 =fork();
}
clock_t begin=clock();
double time_spent=0.0;

printf("Child proccess: %d, and its parent: %d\n",getpid(),getppid());
clock_t end=clock();
time_spent += (double)(end - begin)*1000 / CLOCKS_PER_SEC;
printf("The elapsed time is %f milliseconds\n", time_spent);

return EXIT_SUCCESS;
}