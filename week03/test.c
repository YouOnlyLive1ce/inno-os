#include <stdio.h>

int main(void){
    int arr1[2]={0,1};
    int* arr2=&arr1[0];
    printf("%d",*arr2);
}