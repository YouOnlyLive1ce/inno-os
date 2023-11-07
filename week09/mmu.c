#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

// Structure defining the Page Table Entry (PTE)
struct PTE
{
    // The page is in physical memory (RAM)
    bool valid;
    // The frame number of the page in the RAM
    int frame;
    // The page should be written to disk
    bool dirty;
    // The page is referenced/requested
    int referenced;
};

struct PTE *pagetable;
pid_t pager_pid;

void print_pagetable(int num_pages)
{
    for (int j = 0; j < num_pages; j++)
    {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
               j, pagetable[j].valid, pagetable[j].frame, pagetable[j].dirty, pagetable[j].referenced);
    }
    printf("--------------------------\n");
}

void signal_handler(int signum)
{
    if (signum == SIGCONT)
        return; // No action needed for SIGCONT signal
}

int main(int argc, char *argv[])
{
    signal(SIGCONT, signal_handler);

    int P = atoi(argv[1]); // Total number of pages
    char *memory_accesses = argv[2];
    pager_pid = (pid_t)atoi(argv[3]);

    struct stat st = {0};
    if (stat("/tmp/ex2", &st) == -1)
        mkdir("/tmp/ex2", 0700);

    // Map the pagetable to a file
    int file_write = open("/tmp/ex2/pagetable", O_RDWR);
    pagetable = mmap(NULL, sizeof(struct PTE) * P, PROT_WRITE | PROT_READ, MAP_SHARED, file_write, 0);

    int i = 0;
    int hits = 0;
    int requests = 0;
    while (i < strlen(memory_accesses))
    {
        char mode = memory_accesses[i];
        int page = memory_accesses[i + 1] - '0';
        if (mode == 'W')
            printf("Write Request for page %d\n", page);
        else if (mode == 'R')
            printf("Read Request for page %d\n", page);

        requests++;
        if (pagetable[page].valid == 1)
        {
            hits++;
            printf("It is a valid page\n");
            if (mode == 'W')
            {
                printf("It is a write request, then set the dirty field\n");
                pagetable[page].dirty = 1;
            }
        }
        else
        {
            printf("It is not a valid page --> page fault\nAsk pager to load it from disk (SIGUSR1 signal) and wait\n");
            pagetable[page].referenced = getpid(); // Mark the page reference
            kill(pager_pid, SIGUSR1);              // Signal the pager to load the page from disk
            pause();                               // Wait for pager to response
            printf("MMU resumed by SIGCONT signal from pager\n");
        }
        printf("Page table\n");
        print_pagetable(P);
        i += 3; // Read next mode page
    }

    printf("Done all requests.\nMMU sends SIGUSR1 to the pager.\n");
    kill(pager_pid, SIGUSR1);
    printf("MMU terminates.\n");
    
    

    FILE *results = fopen("ex1.txt", "a");
    double hit_ratio = (double)hits / (double)requests;
    fprintf(results, "Hit ratio: %lf\n", hit_ratio);
    fclose(results);
    munmap(pagetable, sizeof(struct PTE) * P);
    close(file_write);
    return 0;
}
