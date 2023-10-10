#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

typedef struct prime_request {
    int a, b;
} prime_request;

int primes_count ( int a , int b )
{
    int ret = 0;
    for ( int i = a ; i < b ; i ++)
        if ( is_prime ( i ) != 0)
            ret ++;
    return ret ;
}

void *prime_counter(void *arg) {
    prime_request *req = (prime_request *)arg;
    int * count =malloc(sizeof(int));
    *count = primes_count(req->a, req->b);
    return (void *) count;
}

int main(int argc, char *argv[]) {
    //input from command line
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int segment_size = (n + m - 1) / m;

    pthread_t *threads = malloc(m * sizeof(pthread_t));
    prime_request *requests = malloc(m * sizeof(prime_request));
    void **results = malloc(m * sizeof(void *));

    for (int i = 0; i < m; i++)
    {
        requests[i].a = i * segment_size;
        if (requests[i].b > n)
            requests[i].b = n;
        else 
            requests[i].b = (i + 1) * segment_size;
        pthread_create(&threads[i], NULL, &prime_counter, &requests[i]);
    }

    for (int i = 0; i < m; i++)
        pthread_join(threads[i], &results[i]);

    int sum_result = 0;
    for (int i = 0; i < m; i++)
        sum_result += *(int *)results[i];

    free(threads);
    free(requests);
    // for(int i = 0; i < m; ++i)
    //     free(results[i]);
    free(results);

    printf("%d\n", sum_result);

    return 0;
}
