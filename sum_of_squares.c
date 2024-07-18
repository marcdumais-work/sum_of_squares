#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int MAX_WORKERS = 16;

typedef struct {
    unsigned long long start;
    unsigned long long end;
    unsigned long long result;
    double time_taken;
} ThreadData;

void* compute_sum_of_squares(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    struct timeval start_time, end_time;

    gettimeofday(&start_time, NULL);
    data->result = 0;
    for (long i = data->start; i <= data->end; ++i) {
        data -> result += (unsigned long long)i * i;
    }
    gettimeofday(&end_time, NULL);
    data->time_taken = (end_time.tv_sec - start_time.tv_sec) +
                       (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    pthread_exit(NULL);
}

double work(int num_workers, unsigned long long start_num, unsigned long long end_num, pthread_t *threads, ThreadData **thread_data) {
    struct timeval start_time, end_time;
    unsigned long long chunk_size = (end_num - start_num + 1) / num_workers;

    gettimeofday(&start_time, NULL);
    for (int i = 0; i < num_workers; ++i) {
        thread_data[i]->start = start_num + i * chunk_size;
        thread_data[i]->end = thread_data[i]->start + chunk_size - 1UL;
        if (i == num_workers - 1) {
            thread_data[i]->end = end_num; // Ensure last worker handles any remainder
        }

        pthread_create(&threads[i], NULL, compute_sum_of_squares, thread_data[i]);
    }

    for (int i = 0; i < num_workers; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_time, NULL);
    return (end_time.tv_sec - start_time.tv_sec) +
           (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
}

int main(int argc, char* argv[]) {
    pthread_t threads[MAX_WORKERS];
    ThreadData* thread_data[MAX_WORKERS];
    double total_worker_time = 0;
    double total_time = 0;
    unsigned long long total_work_done = 0;
    unsigned long long start_num = 100L;
    unsigned long long end_num = 5000000000UL;

    for (int i = 1; i <= MAX_WORKERS; i++) {
        // Allocate thread_data for each worker dynamically
        for (int j = 0; j < i; j++) {
            thread_data[j] = (ThreadData*)malloc(sizeof(ThreadData));
            if (thread_data[j] == NULL) {
                perror("Failed to allocate memory for thread_data");
                exit(EXIT_FAILURE);
            }
        }

        double iteration_worker_time = 0;
        double total_time_inner = work(i, start_num, end_num, threads, thread_data);
        total_time += total_time_inner;
        for (int j = 0; j < i; j++) {
            iteration_worker_time += thread_data[j]->time_taken;
            total_work_done += thread_data[j]->result;
            total_worker_time += thread_data[j]->time_taken;
        }
        printf("Time for %d workers (wall time, total workers time): %f / %f\n",
               i, total_time_inner, iteration_worker_time);

        // Free thread_data for each worker
        for (int j = 0; j < i; j++) {
            free(thread_data[j]);
        }
    }

    printf("Total work done (sum of squares from %llu to %llu): %llu\n", start_num, end_num, total_work_done);
    printf("Total time taken: %.4f seconds\n", total_time);
    printf("Cumulative time for all workers: %.4f seconds\n", total_worker_time);

    return 0;
}