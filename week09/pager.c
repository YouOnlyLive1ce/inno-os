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

#define PAGESIZE 8
char *replacement;

struct PTE {
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
int P, F, amount_free_frames, amount_disk_accesses = 0;
char **DISK, **RAM;

void print_RAM(){
    printf("RAM array\n");
    for (int i = 0; i < F; i++)
        printf("Frame %d ---> %s\n", i, RAM[i]);
    printf("--------------------------\n");
}

void print_DISK(){
    printf("Disk array\n");
    for (int i = 0; i < P; i++)
        printf("Page %d ----> %s\n", i, DISK[i]);
    printf("--------------------------\n");
}

void print_pagetable(int num_pages) {
    for (int j = 0; j < num_pages; j++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",
            j, pagetable[j].valid, pagetable[j].frame, pagetable[j].dirty, pagetable[j].referenced);
    }
    printf("--------------------------\n");
}

int random() {
    // Generate a random index within the size of the page table
    int index = rand() % F;
    return index;
}

int nfu() {
    int min_count = pagetable[0].referenced; 
    int min_index = 0;

    // Find the page with the minimum reference count
    for (int i = 1; i < P; ++i) {
        if (pagetable[i].referenced < min_count) {
            min_count = pagetable[i].referenced;
            min_index = i;
        }
    }

    return min_index;
}

int aging() {
    int min_age = pagetable[0].referenced;
    int min_index = 0;

    // Find the page with the minimum age (lowest referenced bit value)
    for (int i = 1; i < P; ++i) {
        if (pagetable[i].referenced < min_age) {
            min_age = pagetable[i].referenced;
            min_index = i;
        }
    }

    // Right shift the referenced bit for all pages
    for (int i = 0; i < P; ++i) {
        pagetable[i].referenced >>= 1;
    }

    return min_index;
}

int victim_index(){
    if (replacement == "nfu"){
        return nfu();
    } else if (replacement == "aging"){
        return aging();
    } else {
        return random();
    }
}

void signal_handler(int signum){
    if (signum != SIGUSR1) 
        return;
    printf("--------------------------\n");
    printf("A disk access request from MMU Process (pid=%d)\n", getpid());

    // Handle the requested pages and manage RAM and disk
    for (int i = 0; i <= P; i++){
        if (pagetable[i].referenced != 0){
            printf("Page %d is referenced\n", i);
            if (amount_free_frames > 0){
                pagetable[i].valid = 1;
                int frame = F - amount_free_frames;
                pagetable[i].frame = frame;
                strcpy(RAM[frame], DISK[i]);
                amount_disk_accesses++;
                amount_free_frames--;
                printf("We can allocate it to free frame %d\n", frame);
                printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", i, frame);
            }
            else{
                printf("We do not have free frames in RAM\nChoose a random victim page");
                int victim_page = victim_index();
                printf("%d\n", victim_page);
                if (pagetable[victim_page].dirty==1){
                    strcpy(DISK[victim_page], RAM[pagetable[victim_page].frame]);
                    amount_disk_accesses++;
                }
                printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, pagetable[victim_page].frame);
                pagetable[victim_page].valid = 0;
                pagetable[victim_page].dirty = 0;
                pagetable[victim_page].referenced = 0;
                pagetable[i].frame = pagetable[victim_page].frame;
                pagetable[i].valid = 1;
                strcpy(RAM[pagetable[i].frame], DISK[i]);
                amount_disk_accesses++;
            }
            print_RAM();
            pagetable[i].dirty = 0;
            pagetable[i].referenced = 0;
            kill(pagetable[i].referenced, SIGCONT);
            printf("disk accesses is %d so far\n", amount_disk_accesses);
            printf("Resume MMU process\n--------------------------\n");
            return;
        }
    }
    //cleanup
    munmap(pagetable, sizeof(struct PTE) * P);
    unlink("/tmp/ex2/pagetable");
    for (int i = 0; i < F; i++)
        free(RAM[i]);
    for (int i = 0; i < P; i++)
        free(DISK[i]);

    printf("%d disk accesses in total\n", amount_disk_accesses);
    printf("Pager is terminated\n");
    exit(0);
}


int main(int argc, char *argv[]){
    // Retrieving values from command line arguments
    P = atoi(argv[1]); // Total number of pages
    F = atoi(argv[2]); // Total number of frames
    amount_free_frames = F;

    DISK = (char **)malloc(P * sizeof(char *));
    RAM = (char **)malloc(F * sizeof(char *));

    // Allocate memory for individual pages in RAM and disk
    for (int i = 0; i < F; i++)
        RAM[i] = (char *)malloc(PAGESIZE * sizeof(char));
    for (int i = 0; i < P; i++)
        DISK[i] = (char *)malloc(PAGESIZE * sizeof(char));

    // Create and set up file for pagetable
    int file = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, 0666);
    ftruncate(file, sizeof(struct PTE) * P);
    pagetable = mmap(NULL, sizeof(struct PTE) * P, PROT_WRITE | PROT_READ, MAP_SHARED, file, 0);

    // Initializing disk with random data and setting initial pagetable values
    for (int i = 0; i < P; i++){
        for (int j = 0; j < PAGESIZE - 1; j++)
            DISK[i][j] = ((1337 + rand()) % 95)+32;
        DISK[i][PAGESIZE - 1] = '\0';
        pagetable[i].frame = -1;
    }

    // Display initial status of pagetable, RAM, and disk
    printf("--------------------------\nInitialized page table\n");
    print_pagetable(P);
    printf("Initialized RAM\n");
    print_RAM();
    printf("Initialized disk\n");
    print_DISK();

    signal(SIGUSR1, signal_handler);
    printf("Pager initialized. PID: %d\n", getpid());

    // Continuously wait for signals and handle them
    while (1)
        sleep(1);
    wait(NULL);
    return 0;
}
