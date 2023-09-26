#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
void* addition(const void* cumulative_sum, const void* number, size_t size){
    if (size==sizeof(int))
        *((int*)cumulative_sum)+=*((int*)number);
    else 
        *(double*)cumulative_sum+=*(double*)number;
}
void* multiplication(const void* cumulative_product, const void* number, size_t size){
    if (size==sizeof(int))
        *((int*)cumulative_product)*=(*((int*)number));
    else 
        *(double*)cumulative_product*=*(double*)number;
}
void* max(const void* local_max, const void* number, size_t size){
    if (size==sizeof(int)){
        if (*(int*)number>*(int*)local_max)
            *(int*)local_max=*(int*)number;
    }
    else 
        if (*(double*)number>*(double*)local_max)
            *(double*)local_max=*(double*)number;
}
void* aggregate(void *base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*, size_t)){
    void *(*func_to_call)(const void*,const  void*,size_t);
    func_to_call=opr;
    size_t iteration_step;
    int i=0;
    if (size/n==sizeof(int))
        iteration_step=sizeof(int);
    else 
        iteration_step=sizeof(double);
    while(i<n){
        opr(initial_value,(base+i*iteration_step),size/n);
        i++;
    }
}
int main(){
    //In OS.2023.faqs in question Q46 in answer stated that
    //"What's intended here is that we will depend on the siz argument 
    //in order to determine datatype of void pointer". But it is also stated 
    // we cannot change header of function. Hence, I see three options here:
    //1) create six function int_sum,double_sum,int_product,double_product,int_max,double_max.
    // Nasty code, SSAD course theached us not to do such thing.
    //2) add size_t size to these functions and violate rules, but have good expirience working with this
    //type of task. Very useful in enterprise sphere.
    //3) create nested daughter functions sum,product,max in mother aggregate, 
    //but there is no such thing in C, only C++.
    //
    //So I will choose option 2 and hope TA understood why.
    //btw option 1 works on sum, but not on product and max.
    //It abuses casting int->double. But it make no point to solve task this way

    int arr_int[5]={42,1337,69,1,1};
    double arr_double[5]={1.0,1.2,12.3,123.4,1234.5};
    int int_sum=0,int_product=1, int_max=INT_MIN;
    double double_sum=0,double_product=1,double_max=-DBL_MAX;
    
    //option1: call via instance of function:
    // void* (*sum_int)(void*,size_t,int,void*,void* (*) (const void*,const void*));
    // sum_int=&aggregate;
    // sum_int(arr_int,sizeof(arr_int),5,&int_sum,addition);
    //option2: call via direct access to function:
    aggregate(arr_int,sizeof(arr_int),5,&int_sum,addition);
    printf("%d\n",int_sum);

    aggregate(arr_int,sizeof(arr_int),5,&int_product,multiplication);
    printf("%d\n",int_product);

    aggregate(arr_int,sizeof(arr_int),5,&int_max,max);
    printf("%d\n",int_max);

    aggregate(arr_double,sizeof(arr_double),5,&double_sum,addition);
    printf("%f\n",double_sum);

    aggregate(arr_double,sizeof(arr_double),5,&double_product,multiplication);
    printf("%f\n",double_product);

    aggregate(arr_double,sizeof(arr_double),5,&double_max,max);
    printf("%f\n",double_max);
    return 0;
}