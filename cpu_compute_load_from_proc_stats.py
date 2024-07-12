# Example usage: compute CPU load for 5s:
# $ python3 cpu_compute_load_from_stats.py --interval 5

import time
import argparse

def read_cpu_stats():
    with open('/proc/stat', 'r') as f:
        lines = f.readlines()
    cpu_stats = {}
    for line in lines:
        if line.startswith('cpu'):
            parts = line.split()
            key = parts[0]
            values = list(map(int, parts[1:]))
            cpu_stats[key] = values
    return cpu_stats

def calculate_deltas(initial, final):
    deltas = {}
    for key in final:
        deltas[key] = [final[key][i] - initial[key][i] for i in range(len(final[key]))]
    return deltas

def calculate_cpu_usage(deltas):
    usage = {}
    for key in deltas:
        total_time = sum(deltas[key])
        if total_time == 0:
            continue
        usage[key] = {
            'user': (deltas[key][0] / total_time) * 100,
            'nice': (deltas[key][1] / total_time) * 100,
            'system': (deltas[key][2] / total_time) * 100,
            'idle': (deltas[key][3] / total_time) * 100,
            'total': ((total_time - deltas[key][3]) / total_time) * 100,
        }
    return usage

def main(interval):
    # Initial read
    initial_stats = read_cpu_stats()

    # Wait for the specified interval
    time.sleep(interval)

    # Final read
    final_stats = read_cpu_stats()

    # Calculate deltas
    deltas = calculate_deltas(initial_stats, final_stats)

    # Calculate CPU usage percentages
    cpu_usage = calculate_cpu_usage(deltas)

    # Print results
    print(f"{'CPU':<8} {'User':>8} {'Nice':>8} {'System':>8} {'Idle':>8} {'Productive':>12}")
    print("---------------------------------------------------------")
    for key, value in cpu_usage.items():
        print(f"{key:<8} {value['user']:>7.2f}% {value['nice']:>7.2f}% {value['system']:>7.2f}% {value['idle']:>7.2f}% {value['total']:>11.2f}%")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Calculate CPU usage over a specified interval.")
    parser.add_argument("--interval", type=float, default=1.0, help="Time interval between measurements in seconds.")
    args = parser.parse_args()

    main(args.interval)
