# usage example: Create intense CPU load using 1 worker:
# $ python3 ./compute_sum_of_squares.py --workers 1 --start 1 --end 1000000000

import argparse
import multiprocessing
import time

def compute_sum_of_squares(start, end):
    """Compute the sum of squares from start to end (inclusive)."""
    total_sum = 0
    for i in range(start, end + 1):
        total_sum += i * i
    return total_sum

def worker_process(start, end):
    """Worker process to compute sum of squares."""
    start_time = time.time()
    result = compute_sum_of_squares(start, end)
    end_time = time.time()
    worker_time = end_time - start_time
    return result, worker_time

def generate_load(num_workers, start_num, end_num):
    pool = multiprocessing.Pool(processes=num_workers)
    chunk_size = (end_num - start_num + 1) // num_workers
    results = []

    start_time = time.time()

    for i in range(num_workers):
        worker_start = start_num + i * chunk_size
        worker_end = worker_start + chunk_size - 1
        if i == num_workers - 1:
            worker_end = end_num  # Ensure last worker handles any remainder

        results.append(pool.apply_async(worker_process, (worker_start, worker_end)))

    total_work_done = 0
    total_worker_time = 0

    for result in results:
        result_value, worker_time = result.get()
        total_work_done += result_value
        total_worker_time += worker_time

    pool.close()
    pool.join()

    end_time = time.time()
    total_time = end_time - start_time

    print(f"Total work done (sum of squares from {start_num} to {end_num}): {total_work_done}")
    print(f"Total time taken: {total_time:.4f} seconds")
    print(f"Cumulative time for all workers: {total_worker_time:.4f} seconds")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate CPU load by computing sum of squares.")
    parser.add_argument("--workers", type=int, default=4, help="Number of worker processes.")
    parser.add_argument("--start", type=int, default=1, help="Starting number for sum of squares.")
    parser.add_argument("--end", type=int, default=100000, help="Ending number for sum of squares.")
    args = parser.parse_args()

    generate_load(args.workers, args.start, args.end)
