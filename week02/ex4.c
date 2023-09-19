#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_LIMIT 256

int count(char char_to_find, char str[MAX_LIMIT], int length){
    int counter=0;
    for (int i=0;i<length;i++){
        if (str[i]==tolower(char_to_find) || str[i]==toupper(char_to_find))
            counter++;
    }
    return counter;
}
void countAll(char str[MAX_LIMIT], int length){
    for (int i=0;i<length-1;i++){
        if (str[i]==',')
            break;
        else 
            printf("%c : %d, ",str[i], count(str[i],str,length));
    }
}

int main(void){
    char str[MAX_LIMIT];
  fgets(str, MAX_LIMIT, stdin);

    int length=0;
    bool comma=false;
    while (str[length]){
        length++;
        if (str[length]==',')
            comma=true;
    }

    //Get count for every letter
    if (comma==false)
        countAll(str,length);

    //Get count for particular
    //Put empty space after comma, by requirementi
    if (comma==true){
        char char_to_find;
        int i=0;
        for (i=0;i<256;i++)
            if (str[i]==','){
                char_to_find=str[i+2];
                break;
            }
        printf("%c : %d",char_to_find, count(char_to_find,str,i));
    }
}