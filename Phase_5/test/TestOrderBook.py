import matplotlib.pyplot as plt

order_sizes = [1000, 5000, 10000, 50000, 100000]
execution_times = [0.002, 0.015, 0.030, 0.120, 0.250]  # Replace with actual benchmarks

plt.plot(order_sizes, execution_times, marker='o', linestyle='-', color='b')
plt.xlabel("Number of Orders")
plt.ylabel("Execution Time (seconds)")
plt.title("HFT Order Book Performance")
plt.grid()
plt.show()

# Execution times (Unoptimized): 0.0046684, 1.87e-05, 1.61e-05, 1.63e-05, 1.63e-05 seconds 
# Execution times (Unoptimized): 1.4e-05, 1.12e-05, 1.07e-05, 1.08e-05, 1.09e-05 seconds