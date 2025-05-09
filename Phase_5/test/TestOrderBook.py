import matplotlib.pyplot as plt

order_sizes = [1000, 5000, 10000, 50000, 100000]
execution_times = [0.002, 0.015, 0.030, 0.120, 0.250]  # Replace with actual benchmarks

plt.plot(order_sizes, execution_times, marker='o', linestyle='-', color='b')
plt.xlabel("Number of Orders")
plt.ylabel("Execution Time (seconds)")
plt.title("HFT Order Book Performance")
plt.grid()
plt.show()