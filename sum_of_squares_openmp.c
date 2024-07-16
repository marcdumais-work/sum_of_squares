#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define MAX_WORKERS 32

volatile double total_worker_time = 0;
volatile long long unsigned total_work_done = 0;

double compute_sum_of_squares(int num_workers, unsigned long long start_num, unsigned long long end_num) {
    struct timeval start_time, end_time;
    unsigned long long work_done = 0;
    double worker_times[MAX_WORKERS] = {0};

    gettimeofday(&start_time, NULL);

    #pragma omp parallel num_threads(num_workers)
    {
        int thread_id = omp_get_thread_num();
        unsigned long long chunk_size = (end_num - start_num + 1) / num_workers;
        unsigned long long start = start_num + thread_id * chunk_size;
        unsigned long long end = (thread_id == num_workers - 1) ? end_num : start + chunk_size - 1;
        unsigned long long result = 0;

        struct timeval thread_start_time, thread_end_time;
        gettimeofday(&thread_start_time, NULL);

        for (unsigned long long i = start; i <= end; ++i) {
            result += i * i;
        }

        gettimeofday(&thread_end_time, NULL);
        worker_times[thread_id] = (thread_end_time.tv_sec - thread_start_time.tv_sec) +
                                  (thread_end_time.tv_usec - thread_start_time.tv_usec) / 1000000.0;

        work_done += result;
    }

    gettimeofday(&end_time, NULL);
    double total_time = (end_time.tv_sec - start_time.tv_sec) +
                        (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    double curent_worker_time = 0.0;
    for (int i = 0; i < num_workers; i++) {
        curent_worker_time += worker_times[i];
    }

    printf("Time for %d workers (wall time, total workers time): %f / %f\n",
           num_workers, total_time, curent_worker_time);
    #pragma omp atomic
    total_worker_time += curent_worker_time;
    #pragma omp atomic
    total_work_done += work_done;
    return total_time;
}

int main(int argc, char* argv[]) {
    double total_time = 0;
    unsigned long long start_num = 100L;
    unsigned long long end_num = 5000000000UL;

    for (int i = 1; i <= MAX_WORKERS; i++) {
        total_time += compute_sum_of_squares(i, start_num, end_num);
    }

    printf("Total work done (sum of squares from %llu to %llu): %llu\n", start_num, end_num, total_work_done);
    printf("Total time taken: %.4f seconds\n", total_time);
    printf("Cumulative time for all workers: %.4f seconds\n", total_worker_time);

    return 0;
}
