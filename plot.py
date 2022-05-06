import matplotlib.pyplot as plt
import numpy as np

# note: fail cases are max_iter

# child parallel, across tree sequential
# times1 = [0.409439, 0.187449, 1.047418, 0.490146, 0.591602, 2.336565, 3.826518, 3.732100, 0.181172, 2.634602, 3.441199, 1.213748, 0.329105, 1.931428, 3.677571, 1.823813, 1.795152, 1.426968, 3.924940, 0.237357]
# iters1 = [2, 1, 10, 4, 5, 23, 39, 39, 0, 27, 35, 11, 2, 17, 38, 17, 17, 13, 39, 1]

# within tree sequential, across tree parallel
# times2 = [1.098235, 1.505543, 2.193486, 1.027855, 1.445744, 1.511822, 3.393896, 2.370729, 3.028569, 1.405078, 2.639670, 4.084626, 1.643344, 3.731839, 2.943391, 3.416558, 2.057519, 2.332687, 1.856287, 1.210380]
# iters2 = [11, 11, 29, 12, 13, 12, 39, 31, 35, 14, 31, 34, 15, 36, 34, 37, 24, 22, 15, 15]

# times1 = []
# iters1 = []
# with open("result1.txt", "r") as f:
#     lines = f.readlines()
# for line in lines:
#     if line.startswith("total time:"):
#         times1.append(float(line[len("total time: "):-2]))
#     elif line.startswith("success on iter"):
#         iters1.append(int(line[len("success on iter "):-1]))
#     elif line.startswith("failed"):
#         iters1.append(39)
#
# times2 = []
# iters2 = []
# iter_counter = 0
# with open("result2.txt", "r") as f:
#     lines = f.readlines()
# for line in lines:
#     if line.startswith("total time:"):
#         times2.append(float(line[len("total time: "):-2]))
#         iters2.append(iter_counter - 1)
#         iter_counter = 0
#     elif line.startswith("computation time:"):
#         iter_counter += 1
#
# plt.hist(times1, bins=20, range=(0, 5), alpha=0.7, label="within child")
# plt.hist(times2, bins=20, range=(0, 5), alpha=0.7, label="over trees")
# print(min(times1), max(times1))
# print(min(times2), max(times2))
# # logbins = np.geomspace(0.01, 25, 30)
# # plt.hist(times1, bins=logbins, alpha=0.7, label="within child")
# # plt.hist(times2, bins=logbins, alpha=0.7, label="over trees")
# # plt.xscale('log')
# plt.legend()
# plt.xlabel("Time (seconds)")
# plt.ylabel("Count")
# plt.show()
#
# plt.scatter(iters1, times1, label="within child")
# plt.scatter(iters2, times2, label="over trees")
# plt.legend()
# plt.xlabel("Number of Iterations")
# plt.ylabel("Time (seconds)")
# plt.show()

# counts = []
# with open("large3_postorderlevels.txt", "r") as f:
#     lines = f.readlines()
# for line in lines:
#     counts.append(int(line.split()[-1]))
#
# plt.hist(counts, bins=11, range=(0, 55))
# plt.xlabel("Number of Childen in Level")
# plt.ylabel("Frequency of Levels")
# plt.title("Random Tree 3")
# plt.show()

seq = [4.548, 3.058, 11.300, 11.976, 9.165]
thread2 = [4.864, 3.357, 11.842, 11.839, 9.534]
thread4 = [2.926, 1.896, 7.344, 6.644, 5.977]
thread8 = [1.633, 1.086, 4.303, 3.970, 4.918]
names = ["random 1", "random 2", "random 3", "ladder 1000", "star"]
threads = [1, 2, 4, 8]

times = np.array([seq, thread2, thread4, thread8])
speedups = (1 / (times / times[0])).T

print(speedups)

for i in range(5):
    plt.plot(threads, speedups[i], label=names[i])
plt.legend()
plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.show()
