#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define PASSWORD_LENGTH 8

void generatePassword(char* password) {
    FILE *random = fopen("/dev/urandom", "r");
    if (random == NULL) {
        perror("Error opening /dev/urandom");
        exit(EXIT_FAILURE);
    }

    char characters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    int max_index = strlen(characters);

    password[0] = 'p';
    password[1] = 'a';
    password[2] = 's';
    password[3] = 's';
    password[4]=':';
    for (int i = 5; i < PASSWORD_LENGTH + 5; i++) {
        int random_index = fgetc(random) % max_index;
        password[i] = characters[random_index];
    }

    fclose(random);
}

int main() {
    char* password = (char*) malloc(PASSWORD_LENGTH+5); 
    generatePassword(password); // Generate the password in heap

    FILE* fd = fopen("/tmp/ex1.pid", "w");
    if (fd == NULL) {
        perror("Error opening /tmp/ex1.pid");
        exit(EXIT_FAILURE);
    }
    fprintf(fd, "%d", getpid());
    fclose(fd);

    while (1) {
        // Infinite loop
    }

    return 0;
}
