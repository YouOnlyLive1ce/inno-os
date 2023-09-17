# include <stdio.h>
# include <limits.h>
# include <float.h>
int main(){
    int my_int;
    unsigned short int my_unsigned_short_int;
    signed long int my_signed_long_int;
    float my_float;
    double my_double;

    my_int=INT_MAX;
    my_unsigned_short_int=USHRT_MAX;
    my_signed_long_int=LONG_MAX;
    my_float=FLT_MAX;
    my_double=DBL_MAX;

    printf("%lu %d \n", sizeof(my_int), my_int);
    printf("%lu %hu \n", sizeof(my_unsigned_short_int), my_unsigned_short_int);
    printf("%lu %lu \n", sizeof(my_signed_long_int), my_signed_long_int);
    printf("%lu %f \n", sizeof(my_float), my_float);
    printf("%lu %f \n", sizeof(my_double), my_double);
    return 0;
}
