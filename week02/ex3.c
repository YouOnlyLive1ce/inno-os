#include <stdio.h>
#include <math.h>

void convert(unsigned long long number_to_convert, unsigned short from_system, unsigned short to_system){
    //Convert given number to decimal
    unsigned long long number_to_convert_in_decimal=0;
    short digit_iterator=0;
    while (number_to_convert>0){
        number_to_convert_in_decimal+=(number_to_convert%10)*pow(from_system,digit_iterator);
        digit_iterator++;
        number_to_convert/=10;
    }
    
    //Create array of chars to save every digit of converted number
    unsigned short converted_number_amount_digit=((log(number_to_convert_in_decimal))/(log(to_system)))+1;
    char converted_number[converted_number_amount_digit];
    digit_iterator=converted_number_amount_digit-1;
    //Devide and save the remainder of the division until number is no longer divisible
    while (number_to_convert_in_decimal>=1)
    {
        converted_number[digit_iterator]=number_to_convert_in_decimal%to_system+'0';
        number_to_convert_in_decimal/=to_system;
        digit_iterator--;
    }
    //Output result
    digit_iterator=0;
    while (digit_iterator<converted_number_amount_digit){
        printf("%c",converted_number[digit_iterator]);
        digit_iterator++;
    }
}

int main(void){
    unsigned long long number_to_convert; 
    unsigned short from_system, to_system;
    printf("Enter non-negative number to convert x, from numerical system s to numerical system t. s,t=[2,10]\n");
    scanf("%llu %hu %hu", &number_to_convert, &from_system, &to_system);

    if (from_system>=2 && from_system<=10 && to_system>=2 && to_system<=10){
        int amount_digits=0;
        amount_digits=log10(number_to_convert)+1;
        char number_to_convert_str[amount_digits];
        sprintf(number_to_convert_str,"%llu", number_to_convert);
        for (short restricted_digit=from_system;restricted_digit<=9;restricted_digit++)
            for (int i=0;i<amount_digits;i++)
                if (number_to_convert_str[i]=='0'+restricted_digit){
                    printf("Cannot convert!");
                    return 1;
                }
        convert(number_to_convert,from_system,to_system);
    } else {
        printf("Cannot convert!");
        return 1;
    }
    return 0;
}