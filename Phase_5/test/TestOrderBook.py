import matplotlib.pyplot as plt


order_sizes = [100000, 500000, 1000000, 5000000, 10000000]

execution_timesU = [0.0267708, 0.0133576, 0.0150353, 0.0115099, 0.0116154]
execution_timesO = [0.0038034, 0.0038369, 0.0035081, 0.0035271, 0.0035951]
execution_timesU10 = [0.144303, 0.137007, 0.127737, 0.134206, 0.13139]
execution_timesO10 = [0.0374406, 0.0367814, 0.0373585, 0.0355887, 0.0374482]

plt.plot(order_sizes, execution_timesU, marker='o', linestyle='-', color='b', label='Unoptimized')
plt.plot(order_sizes, execution_timesO, marker='o', linestyle='--', color='r', label='Optimized')
plt.plot(order_sizes, execution_timesU10, marker='o', linestyle='-', color='y', label='Unoptimized 10X')
plt.plot(order_sizes, execution_timesO10, marker='o', linestyle='--', color='g', label='Optimized 10X')
plt.xlabel("Number of Orders")
plt.ylabel("Execution Time (seconds)")
plt.title("HFT Order Book Performance")
plt.legend()
plt.grid()
plt.show()

# base
# Execution times (Unoptimized): 0.0267708, 0.0133576, 0.0150353, 0.0115099, 0.0116154 seconds 
# Execution times (Optimized): 0.0038034, 0.0038369, 0.0035081, 0.0035271, 0.0035951 seconds 

# mult by 10
# Execution times (Unoptimized): 0.144303, 0.137007, 0.127737, 0.134206, 0.13139 seconds 
# Execution times (Optimized): 0.0374406, 0.0367814, 0.0373585, 0.0355887, 0.0374482 seconds 