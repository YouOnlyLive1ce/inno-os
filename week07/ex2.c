#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096 

int main() {
    int fd_text = open("text.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    int fd_devrandom = open("/dev/random", O_RDONLY);
    int FILESIZE=500 * 1024 * 1024;
    ftruncate(fd_text, FILESIZE);
    char *map = (char *)mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_text, 0);
    //buffer is used to improve perfomance
    char buffer[BUFFER_SIZE];
    int charCount = 0;
    int capitalCount = 0;
    while (charCount < FILESIZE) {
        ssize_t bytes = read(fd_devrandom, buffer, BUFFER_SIZE);
        if (bytes <= 0) 
            break;
        for (int i = 0; i < bytes && charCount < FILESIZE; i++) {
            char c = buffer[i];
            if (isprint(c)) {
                map[charCount] = c;
                charCount++;
                if (charCount % 1024 == 0 && charCount < FILESIZE) {
                    map[charCount] = '\n';
                    charCount++;
                }
                if (isupper(c)) 
                    capitalCount++;
            }
        }
    }
    printf("Total capital letters: %d\n", capitalCount);
    for (int i = 0; i < charCount; i++) 
        if (isupper(map[i])) 
            map[i] = tolower(map[i]);
    munmap(map, FILESIZE);

    close(fd_devrandom);
    close(fd_text);
    return 0;
}
