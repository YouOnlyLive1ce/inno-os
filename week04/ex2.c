#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include <time.h>

int main(void){
    FILE * pFile;
    pFile = fopen("temp.txt","w");
    pFile = fopen("temp.txt","a");
    
    int u[120];
    for (int i=0;i<120;i++)
        u[i]=rand()%100;
    int v[120];
    for (int i=0;i<120;i++)
        v[i]=rand()%100;

    int n;
    fscanf(stdin, "%d",&n);
    pid_t parent=getpid();
    pid_t arrProccesses[n]; //i->pid
    //creating n proccesses - childs of parent
    int i=0;
    for (i=0; i<n;i++){
        arrProccesses[i]=fork();
        if (arrProccesses[i]==0){
            break;
        }
    }
    //child proccesses are keep terminating here
    //now every child proccess know his number i
    //execute proccesses
    int sum=0;
    for (int j=(120/n)*i;j<(120/n)*(i+1);j++)
        sum+=(u[j]*v[j]);
    if (getpid()!=parent)//otherwise some random number will be printed
        fprintf(pFile,"%d\n", sum);
    //exit all non-parent proccesses
    if (getpid()!=parent)
        exit (0);
    //read and sum in one value
    int dot_product=0;
    pFile = fopen("temp.txt","r");
    for (int i=0;i<n;i++){
        fscanf(pFile,"%d^\n",&sum);
        dot_product+=sum;
    }
    printf("%d",dot_product);
    return 0;
}