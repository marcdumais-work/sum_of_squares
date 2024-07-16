# Sum of Squares

## Build

With or without optimization:

```bash
# with
gcc -O1 -pthread -o sum_of_squares sum_of_squares.c

# without
gcc -pthread -o sum_of_squares sum_of_squares.c
```

## Execute

```bash
$ ./sum_of_squares
Time for 1 workers (wall time, total workers time): 2.582437 / 2.581940
Time for 2 workers (wall time, total workers time): 1.310680 / 2.613840
Time for 3 workers (wall time, total workers time): 1.069902 / 2.937649
Time for 4 workers (wall time, total workers time): 0.743361 / 2.875449
Time for 5 workers (wall time, total workers time): 0.758792 / 3.194261
Time for 6 workers (wall time, total workers time): 0.664267 / 3.597698
Time for 7 workers (wall time, total workers time): 0.618582 / 3.958804
Time for 8 workers (wall time, total workers time): 0.559551 / 4.396329
Time for 9 workers (wall time, total workers time): 0.624731 / 4.859120
Time for 10 workers (wall time, total workers time): 0.621696 / 5.207822
Time for 11 workers (wall time, total workers time): 0.606893 / 5.260752
Time for 12 workers (wall time, total workers time): 0.573669 / 5.861300
Time for 13 workers (wall time, total workers time): 0.579399 / 6.468218
Time for 14 workers (wall time, total workers time): 0.595109 / 6.523404
Time for 15 workers (wall time, total workers time): 0.578506 / 7.310590
Time for 16 workers (wall time, total workers time): 0.563451 / 8.010766
Total work done (sum of squares from 100 to 5000000000): 470444716059854368
Total time taken: 13.0510 seconds
Cumulative time for all workers: 75.6579 seconds

```

