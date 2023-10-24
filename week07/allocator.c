#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
unsigned int* memory;
void allocate_first_fit(unsigned int adrs,int size){
    int amount_empty_cells=0;
    for (int i=0;i<10000000;i++){
        if (memory[i]==0)
            amount_empty_cells++;
        else 
            amount_empty_cells=0;
        if (amount_empty_cells==size){
            for (int filled_cells=0;filled_cells<size;filled_cells++)
                memory[i+filled_cells]=adrs;
            break;
        }
    }
}
void allocate_worst_fit(unsigned int adrs,int size){
    if (memory[adrs]==0)
        for (int i=adrs;i<adrs+size;i++)
            memory[i]=adrs;
    else{
        int index_worst_block=-1;
        int worst_block_size=0;
        int found_block_size=0;
        for (int i=0;i<10000000;i++){
            if (memory[i]==0)
                found_block_size++;
            else{
                if (found_block_size>worst_block_size){
                    worst_block_size=found_block_size;
                    index_worst_block=i;
                }
                found_block_size=0;
            }
        }
        for (int i=index_worst_block;i<index_worst_block+size;i++)
            memory[i]=adrs;
    }
}
void allocate_best_fit(unsigned int adrs,int size){
    int best_block_size=0;
    int index_best_block=-1;
    int found_block_size=0;
    for (int i=0;i<10000000;i++){
        if (memory[i]==0)
            found_block_size++;
        else{
            if (found_block_size-size<best_block_size-size){
                best_block_size=found_block_size;
                index_best_block=i;
            }
            found_block_size=0;
        }
    }
    for (int i=index_best_block;i<index_best_block+size;i++){
        memory[i]=adrs;
    }
}
void clear(unsigned int adrs){
    int i=adrs;
    while (i<10000000){
        if (memory[i]==adrs){
            //memory block found, fill with 0
            while (i<10000000){
                if (memory[i]==adrs)
                    memory[i]=0;
                i++;
            }
            break;
        }
        i++;
    }
}
int main(){
    clock_t start,end;
    double total_time,amount_queries=0;
    start=clock();
    char line[32];
    memory = (unsigned int *)calloc(10000000, sizeof(unsigned int));
    unsigned int adrs, size;
    char algo;
    FILE *file = fopen("queries.txt", "r");
    while (fgets(line, sizeof(line), file)) {
        amount_queries++;
        if (strcmp(line, "end") == 0) 
            break;
        else if (sscanf(line, "allocate %u %d", &adrs, &size) == 2) {
            allocate_best_fit(adrs,size);
        } else if (sscanf(line, "clear %u",&adrs) == 1) {
            clear(adrs);
        }
    }
    fclose(file);
    end=clock();
    printf("best_fit: %f, throughput %f\n",((double)(end - start)) / CLOCKS_PER_SEC, amount_queries/(double)(end - start));

    start=clock();
    memory = (unsigned int *)calloc(10000000, sizeof(unsigned int));
    file = fopen("queries.txt", "r");
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "end") == 0) 
            break;
        else if (sscanf(line, "allocate %u %d", &adrs, &size) == 2) {
            allocate_worst_fit(adrs,size);
        } else if (sscanf(line, "clear %u",&adrs) == 1) {
            clear(adrs);
        }
    }
    fclose(file);
    end=clock();
    printf("worst_fit: %f, throughput %f\n",((double)(end - start)) / CLOCKS_PER_SEC, amount_queries/(double)(end - start));

    start=clock();
    file = fopen("queries.txt", "r");
    memory = (unsigned int *)calloc(10000000, sizeof(unsigned int));
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "end") == 0) 
            break;
        else if (sscanf(line, "allocate %u %d", &adrs, &size) == 2) {
            allocate_first_fit(adrs,size);
        } else if (sscanf(line, "clear %u",&adrs) == 1) {
            clear(adrs);
        }
    }
    fclose(file);
    end=clock();
    printf("first_fit: %f, throughput %f\n",((double)(end - start)) / CLOCKS_PER_SEC, amount_queries/(double)(end - start));
    return 0;
}