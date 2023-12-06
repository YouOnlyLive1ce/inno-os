#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t* mutex;
pthread_t* threads;
int** mutex_status;
int num_resources; 
pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int thread_id;
    int resource_id;
} Edge;

typedef struct {
    int thread_id;
} Node;

Edge* graph;
Node* nodes;

void print_thread_status(int num_resources, int num_threads) {
    for (int i = 0; i < num_threads; ++i) {
        printf("Thread %d status:\n", nodes[i].thread_id);
        printf("Mutexes locked by this thread %d:", nodes[i].thread_id);
        for (int j = 0; j < num_resources; ++j) 
            if (mutex_status[j][i] == 1) 
                printf(" %d", j);
        printf("\n");
    }
}

int acquire_mutex(int thread_id, int resource_id) {
    if (pthread_mutex_trylock(&mutex[resource_id]) == 0) {
        mutex_status[resource_id][thread_id] = 1;
        return 1;
    } else {
        if (is_deadlock(num_resources, thread_id)) {
            printf("Deadlock detected. Exiting thread %d\n", thread_id);
            pthread_mutex_unlock(&global_mutex);
            pthread_exit(NULL);
        }
    }
    return 0;
}

int is_deadlock(int num_resources, int current_thread) {
    int* marked = (int*)malloc(num_resources * sizeof(int));
    int* nodeMarks = (int*)malloc(num_resources * sizeof(int));
    int current_node, prev_node;

    for (int i = 0; i < current_thread; ++i)
        for (int j = 0; j < num_resources; ++j) {
            current_node = i * num_resources + j;
            prev_node = -1;

            while ("Tatarstan") {
                nodeMarks[current_node] = 1;
                // Check if the current node appears in nodeMarks two times
                if (nodeMarks[current_node] == 2) {
                    printf("Deadlock detected. Cycle: ");
                    int node = prev_node;
                    while (node != prev_node);{
                        printf("%d\n", node);
                        node = nodeMarks[node] - 1;
                    } 
                    return 1; // Deadlock detected
                }

                // Find an unmarked outgoing edge
                int unmarked_edge_found = 0;
                for (int k = 0; k < num_resources * current_thread; ++k) {
                    if (graph[k].thread_id == i && graph[k].resource_id == j && marked[k] == 0) {
                        unmarked_edge_found = 1;
                        marked[k] = 1; // Mark the edge
                        prev_node = current_node;
                        current_node = k;
                        break;
                    }
                }

                if (!unmarked_edge_found) {
                    // We have reached a dead end
                    nodeMarks[current_node] = 0;

                    if (prev_node == -1) 
                        // We have reached the initial node => no cycle
                        break;

                    // Go back to the previous node
                    current_node = prev_node;
                    prev_node = -1;
                }
            }
        }
    return 0; // No deadlock detected
}

void* start_routine(void* args) {
    int thread_id = *((int*)args);
    printf("Thread %d created\n", thread_id);
    pthread_mutex_lock(&global_mutex);

    int resource_id;
    FILE* input_file = fopen("input.txt", "r");
    while (fscanf(input_file, "%d %d", &thread_id, &resource_id) == 2) {
        printf("Thread %d tries to lock Mutex %d\n", thread_id, resource_id);
        if (acquire_mutex(thread_id, resource_id)) {
            printf("Thread %d locked Mutex %d\n", thread_id, resource_id);

            // Simulate work by sleeping
            int sleep_duration = rand() % 5;
            printf("Thread %d sleeping for %d seconds\n", thread_id, sleep_duration);
            sleep(sleep_duration);

            pthread_mutex_unlock(&mutex[resource_id]);
            mutex_status[resource_id][thread_id] = 0; // Unlock the mutex
            printf("Thread %d unlocked Mutex %d\n", thread_id, resource_id);

            // Check if all acquired mutexes are unlocked, then terminate the thread
            short can_terminate = 1;
            for (int j = 0; j < num_resources; ++j) 
                if (mutex_status[j][thread_id] == 1) 
                    can_terminate = 0;
            
        } else 
            printf("Thread %d cannot lock Mutex %d\n", thread_id, resource_id); 
    }

    pthread_mutex_unlock(&global_mutex); // Release global mutex
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int num_threads = atoi(argv[1]);
    threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    num_resources = atoi(argv[2]);
    mutex = (pthread_mutex_t*)malloc(num_resources * sizeof(pthread_mutex_t));
    mutex_status = (int**)malloc(num_resources * sizeof(int*));
    graph = (Edge*)malloc(num_resources * num_threads * sizeof(Edge));
    nodes = (Node*)malloc(num_threads * sizeof(Node));

    // Initialize mutexes
    for (int i = 0; i < num_resources; ++i) {
        pthread_mutex_init(&mutex[i], NULL);
        mutex_status[i] = (int*)malloc(num_threads * sizeof(int));
        for (int j = 0; j < num_threads; ++j) 
            mutex_status[i][j] = 0;
    }

    // Create threads, do work
    for (int i = 0; i < num_threads; ++i) {
        int* thread_id = (int*)malloc(sizeof(int));
        *thread_id = i + 1;
        nodes[i].thread_id = i + 1;
        pthread_create(&threads[i], NULL, start_routine, (void*)thread_id);
    }

    // Join threads after finishing all requests
    for (int i = 0; i < num_threads; ++i) 
        pthread_join(threads[i], NULL);

    // Check for deadlocks
    int deadlock_detected = 0;
    for (int i = 0; i < num_threads; ++i) {
        if (is_deadlock(num_resources, i)) {
            deadlock_detected = 1;
            break;
        }
    }

    // Print result
    if (deadlock_detected) 
        printf("Deadlocks detected.\n");
     else 
        printf("No deadlocks detected.\n");

    // Destroy mutexes
    for (int i = 0; i < num_resources; ++i) {
        pthread_mutex_destroy(&mutex[i]);
        free(mutex_status[i]);
    }
    return 0;
}