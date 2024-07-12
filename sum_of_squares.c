#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    long start;
    long end;
    long long result;
    double time_taken;
} ThreadData;

void* compute_sum_of_squares(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    struct timeval start_time, end_time;

    gettimeofday(&start_time, NULL);
    data->result = 0;
    for (int i = data->start; i <= data->end; ++i) {
        data->result += (long long)i * i;
    }
    gettimeofday(&end_time, NULL);
    data->time_taken = (end_time.tv_sec - start_time.tv_sec) + 
                       (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    pthread_exit(NULL);
}

double work(int num_workers, long start_num, long end_num, pthread_t *threads, ThreadData *thread_data) {
    struct timeval start_time, end_time;
    int chunk_size = (end_num - start_num + 1) / num_workers;
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < num_workers; ++i) {
        thread_data[i].start = start_num + i * chunk_size;
        thread_data[i].end = thread_data[i].start + chunk_size - 1;
        if (i == num_workers - 1) {
            thread_data[i].end = end_num; // Ensure last worker handles any remainder
        }

        pthread_create(&threads[i], NULL, compute_sum_of_squares, &thread_data[i]);
    }
    
    double total_worker_time = 0;

    for (int i = 0; i < num_workers; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_time, NULL);
    return (end_time.tv_sec - start_time.tv_sec) + 
                        (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
}

int main(int argc, char* argv[]) {
    pthread_t threads[16];
    ThreadData thread_data[16];
    double total_worker_time = 0;
    long long total_work_done = 0;
    double total_time = 0;
    long start_num = 100;
    long end_num = 1000000000;
    for(int i =1; i < 16; i++){

        double total_time_inner = work(i, start_num, end_num, threads, thread_data);
        printf("Time for %d workers: %f\n", i, total_time_inner);
        total_time += total_time_inner;
        for (int j = 0; j < i; j++) {
            total_work_done += thread_data[j].result;
            total_worker_time += thread_data[j].time_taken;
        }
    }
    printf("Total work done (sum of squares from %ld to %ld): %lld\n", start_num, end_num, total_work_done);
    printf("Total time taken: %.4f seconds\n", total_time);
    printf("Cumulative time for all workers: %.4f seconds\n", total_worker_time);

    return 0;
}
