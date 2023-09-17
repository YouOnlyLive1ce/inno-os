#include <stdio.h>
#include <math.h>
int tribonacci(int n){
    int n0=0,n1=1,n2=1,ans=-1,prev_n1,prev_n2;
    for (int i=3;i<=n;i++){
        prev_n1=n1;
        prev_n2=n2;
        n2=n0+n1+n2;
        n1=prev_n2;
        n0=prev_n1;
    }
    return n2;
}
int main(void){
    printf("%d\n",tribonacci(4));
    printf("%d",tribonacci(36));
    return 0;
}