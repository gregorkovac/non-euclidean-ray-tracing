import numpy as np
import os
import matplotlib.pyplot as plt

point_file = open("./points.txt", "r")

points = []

cnt = 0
for line in point_file:
    points.append(np.array([float(x) for x in line.split()]))

    cnt += 1

points = np.array(points)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

pt_limit = 0

for point in points:

    # pt_limit += 1
    # if pt_limit > 1000:
    #     break
    ax.scatter(point[0], point[1], point[2], c='b', marker='o')

ax.set_aspect('equal')
plt.show()

os.remove("./points.txt")