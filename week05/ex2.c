#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

//threads are used for concurrent programs execution
struct Thread {
    pthread_t id;
    int i; // index of created thread
    char message[256];
};

void *routine(void *arg) {
    char message[256]="Hello from thread ";
    char i_str[4];
    //cast void* to struct Thread *
    struct Thread *thread_struct = (struct Thread *)arg;
    sprintf(i_str, "%d", thread_struct->i);
    strcat(message,i_str);
    strcat(message,"\n");
    strcpy(thread_struct->message,message);
    printf("%s",thread_struct->message);
    return NULL;
}

int main() {
    int n;
    scanf("%d", &n);
    struct Thread *threads[n];

    for (int i = 0; i < n; i++) {
        struct Thread *thread_struct = malloc(sizeof(struct Thread));
        thread_struct->i = i;
        printf("Thread %d is created\n", i);
        pthread_create(&thread_struct->id, NULL, routine, thread_struct);
        pthread_join(thread_struct->id, NULL); // Wait for the thread to finish
        threads[i] = thread_struct;
    }

    // Free the allocated memory for each thread
    for (int i = 0; i < n; i++) 
        free(threads[i]);
    return 0;
}
