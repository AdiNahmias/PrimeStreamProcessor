#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_THREADS 4
#define CHUNK_SIZE 2000 // כל תהליך יקבל חתיכה בגודל 2000 מספרים

typedef struct {
    atomic_int total_primes;
    pthread_mutex_t input_mutex;
} SharedData;

typedef struct {
    SharedData* shared_data;
} ThreadArgs;

long long multiply_mod(long long a, long long b, long long mod) {
    long long result = 0;
    a = a % mod;
    while (b > 0) {
        if (b & 1)
            result = (result + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
}

long long modular_exponentiation(long long base, long long exponent, long long mod) {
    if (exponent == 0)
        return 1;
    long long half = modular_exponentiation(base, exponent / 2, mod);
    long long half_squared = (half * half) % mod;
    
    if (exponent % 2 == 0)
        return half_squared;
    else
        return (half_squared * base) % mod;
}

int isPrime(long long p, int iter) {
    if (p < 2 || (p != 2 && p % 2 == 0)) 
        return 0;
    long long s = p - 1;
    while (s % 2 == 0) {
        s /= 2;
    }
    for (int i = 0; i < iter; i++) {
        long long a = rand() % (p - 1) + 1;
        long long temp = s;
        long long mod = modular_exponentiation(a, temp, p);

        while (temp != p - 1 && mod != 1 && mod != p - 1) {
            mod = multiply_mod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
            return 0;
    }
    return 1;
}

// bool isPrime(int n) {
//     if (n <= 1) {
//         return false;
//     }
//     for (int i = 2; i * i <= n; i++) {
//         if (n % i == 0) {
//             return false;
//         }
//     }
//     return true;
// }


void* process_chunk(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    SharedData* shared_data = threadArgs->shared_data;
    int chunk[CHUNK_SIZE];

    while (1) {
        int count = 0;

        pthread_mutex_lock(&(shared_data->input_mutex));
        for (int i = 0; i < CHUNK_SIZE; i++) {
            if (scanf("%d", &chunk[i]) == EOF) {
                break;
            }
            count++;
        }
        pthread_mutex_unlock(&(shared_data->input_mutex));

        if (count == 0) {
            break;
        }

        for (int i = 0; i < count; i++) {
            if (isPrime(chunk[i],4)) {
            // if (isPrime(chunk[i])) {
                atomic_fetch_add(&(shared_data->total_primes), 1);
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    SharedData shared_data;
    atomic_init(&shared_data.total_primes, 0);
    pthread_mutex_init(&shared_data.input_mutex, NULL);

    pthread_t threads[NUM_THREADS];
    ThreadArgs threadArgs[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threadArgs[i].shared_data = &shared_data;
        pthread_create(&threads[i], NULL, process_chunk, &threadArgs[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", atomic_load(&shared_data.total_primes));

    pthread_mutex_destroy(&shared_data.input_mutex);

    return 0;
}

