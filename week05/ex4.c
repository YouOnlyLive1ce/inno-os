# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}
// The mutex
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;
// Do not modify these variables directly , use the functions
// on the right side.
int k = 0; //next number
int c = 0; //primes counter
// input from command line
int n = 0;

// get next prime candidate
int get_number_to_check()
{
    int ret = k ;
    if ( k != n )
        k ++;
    return ret ;
}

// increase prime counter
void increment_primes ()
{
    c++;
}

void *check_primes(void *arg)
{
    int num_to_check;
    while(1) {
        pthread_mutex_lock(&global_lock);
        num_to_check = get_number_to_check();
        pthread_mutex_unlock(&global_lock);
        if (num_to_check>=n)
            break;
        if (is_prime(num_to_check)) {
            pthread_mutex_lock(&global_lock);
            increment_primes();
            pthread_mutex_unlock(&global_lock);
        }
    }
}

int main(int argc, char *argv[])
{
    n = atoi(argv[1]);
    int m =atoi(argv[2]);

    pthread_t *threads = malloc(m * sizeof(pthread_t));
    for (int i = 0; i < m; i++){
        pthread_create(&threads[i], NULL, &check_primes, NULL);
    }
    for (int i = 0; i < m; i++){
        pthread_join(threads[i], NULL);
    }
    free(threads);

    printf("%d\n", c);
    pthread_mutex_destroy(&global_lock);
    exit(EXIT_SUCCESS);
}