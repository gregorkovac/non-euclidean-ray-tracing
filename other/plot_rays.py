import numpy as np
import os
import matplotlib.pyplot as plt

point_file = open("points.txt", "r")

points = []

cnt = 0
for line in point_file:
    points.append(np.array([float(x) for x in line.split()]))

    cnt += 1

points = np.array(points)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')


cnt = 0

color = np.random.rand(3,)
prev = points[0]

param_file = open("include/ParametersAndConstants.h", "r")

for line in param_file:
    if "#define MAX_ITER" in line:
        max_iter = int(line.split()[2])
        break

#max_iter += 1

for point in points:
    if cnt == max_iter:
        cnt = 0
    color = np.random.rand(3,)



    #ax.scatter(point[0], point[1], point[2], c=color, marker='o')
    # Line form prev to point
    if cnt != 0:
        ax.plot([prev[0], point[0]], [prev[2], point[2]], [prev[1], point[1]], c=color)
        #pass
    else:
        #print(point)
        ax.scatter(point[0], point[2], point[1], c=color, marker='o')
    

    prev = point

    cnt += 1

ax.set_aspect('equal')
plt.show()