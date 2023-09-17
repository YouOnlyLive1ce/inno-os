#include <stdio.h>
int main(void){
    //First pattern
    for (int n=1;n<8;n++){
        for (int i=1;i<=n;i++)
            printf("*");
        printf("\n");
    }

    //Second pattern
    int delta=1,n=1;
    while (n<8 && n>0){
        for (int i=1;i<=n;i++)
            printf("*");
        printf("\n");
        if (n>=4)
            delta=-1;
        n+=delta;
    }

    //Third pattern. Really?
    for (int n=1;n<8;n++){
        for (int i=1;i<8;i++)
            printf("*");
        printf("\n");
    }
    // printf("*******\n*******\n*******\n*******\n*******\n*******\n*******\n");
}