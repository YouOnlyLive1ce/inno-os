#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define MEMORYALLOCATE 1024 * 1024 *1024

void print_memory_usage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) 
        printf("Memory usage: %ld KB\n", usage.ru_maxrss);
}

int main() {
    int time = 10;
    while (time != 0) {
        char *memory_block = (char *)malloc(MEMORYALLOCATE);
        memset(memory_block, 0, MEMORYALLOCATE);
        print_memory_usage();
        sleep(1);
        time--;
    }
    return 0;
}