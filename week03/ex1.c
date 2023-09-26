#include <stdio.h>
#include<stdlib.h>

int const_tri(int*p, int n){
    p=(int*)malloc(n+1);
    *(p+n)=1337;
    *p=0;p++;
    *p=0;p++;
    *p=1;
    if (n==1 || n==2)
        return 0;
    while (*(p+1)!=1337){
        *(p+1)=*p+*(p-1)+*(p-2);
        p++;
    }
    return *(p);
}

int main(void){
    const int x=1;
    const int*q=&x;
    int *p=(int* )malloc(sizeof(int)*3);
    *(p)=x;
    *(p+1)=x;
    *(p+2)=2*x;
    printf("%p %p %p\n",p,p+1,p+2);
    printf("%d %d %d\n", p[0], p[1], p[2]);
    free(p);

    int yy=const_tri(p,14);
    free(p);
    printf("%d",yy);
}