# Guaranteed Scheduling
To achieve guaranteed 1/n of cpu time (for n processes logged on):
1. Monitoring the total amount of CPU time per process and the total logged on time
2. Calculating the ratio of allocated cpu time to the amount of CPU time each process is entitled to
3. Running the process with the lowest ratio
4. Switching to another process when the ratio of the running process has passed its “goal ratio”
