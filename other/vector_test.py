import numpy as np
import matplotlib.pyplot as plt

def map_to_range(x, min, max):
    if x < min:
        x = max - (min - x) % (max - min)
    elif x > max:
        x = min + (x - min) % (max - min)

    return x

def xyz_to_uv(x):
    #u = np.arccos(map_to_range(x[2], -1, 1))
    #v = np.arcsin(map_to_range(x[1] / (np.cos(u) + 0.000001), -1, 1))

    # u = np.arcsin(map_to_range(x[2], -1, 1))
    # v = np.arccos(map_to_range(x[0] / (np.sin(u) + 0.000001), -1, 1))

    u = np.arccos(x[2])

    # if (x[2] == -1):
    #     v = 0
    # elif (x[2] == 1):
    #     v = np.pi
    # else:
    v = np.arccos(map_to_range(x[0] / (np.sin(u) + 0.000001), -1, 1))

    #print(u, v)

    return np.array([u, v])

def uv_to_xyz(uv):
    # x = np.cos(uv[1]) * np.cos(uv[0])
    # y = np.sin(uv[1]) * np.cos(uv[0])
    # z = np.cos(uv[0])

    x = np.sin(uv[0]) * np.cos(uv[1])
    y = np.sin(uv[0]) * np.sin(uv[1])
    z = np.cos(uv[0])

    return np.array([x, y, z])

def next_point(x, vel, t):
    #uv = xyz_to_uv(x)

    #vel_u, vel_v = xyz_to_uv(vel)

    gamma_22 = - np.cos(x[0]) * np.sin(x[0])
    gamma_12 = np.cos(x[0]) / (np.sin(x[0]) + 0.000001)

    acc = np.array([
        - gamma_22 * (vel[1] ** 2),
        - 2 * gamma_12 * vel[0] * vel[1],
    ])

    vel_next = vel + t * acc[0]

    x_next = x + t * vel_next

    return x_next, vel_next

#fig = plt.figure()
#ax = fig.add_subplot(111, projection='3d')

# u_space = np.linspace(0, np.pi, 10)
# v_space = np.linspace(0, 2*np.pi, 10)

# u_space_after = []
# v_space_after = []

# one_pass_v = False

# u_cnt = 0

# for u in u_space:
#     #v_space_after = []
#     v_cnt = 0
#     for v in v_space:
#         point = uv_to_xyz(np.array([u, v]))

#         # if np.isnan(point).any():
#         #     print(u, v)
        
#         #ax.scatter(point[0], point[1], point[2], c='r', marker='o')

#         uv_after = xyz_to_uv(point)

#         #print("(", u, ", ", v, ") -> ",  uv_after)

#         #if not one_pass_v:
#            # print(uv_after[1])

#         #print(v_cnt, u_cnt, v_cnt == u_cnt)

#         if u_cnt == v_cnt:
#             v_space_after.append(uv_after[1])
#             print(v_space_after)
        
#         v_cnt += 1

#     u_space_after.append(uv_after[0])

#     u_cnt += 1



# u_space_after = np.array(u_space_after)
# v_space_after = np.array(v_space_after)

# print("u_space:")
# print(u_space)
# print("u_space_after:")
# print(u_space_after)
# print("---------")
# print("v_space:")
# print(v_space)
# print("v_space_after:")
# print(v_space_after)
# print("---------")

# ax2 = fig.add_subplot(122)
# ax2.scatter(u_space, v_space, c='b', marker='o')
# ax2.scatter(u_space_after, v_space_after, c='r', marker='x')

# plt.show()


# exit(0)
origin = np.array([0, 0, 0])
focal_length = 1
image_plane_width, image_plane_height = 1, 1

image_plane_center = origin + np.array([0, focal_length, 0])

image_plane_points = []

for x in np.linspace(-image_plane_width/2, image_plane_width/2, 5):
    for y in np.linspace(-image_plane_height/2, image_plane_height/2, 5):
        point = np.array([x, focal_length, y])
        image_plane_points.append(point)

image_plane_points = np.array(image_plane_points)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.scatter(image_plane_points[:, 0], image_plane_points[:, 1], image_plane_points[:, 2], c='r', marker='o')

ax.scatter(origin[0], origin[1], origin[2], c='b', marker='o')

for point in image_plane_points:
    vec = point - origin

    #ax.plot([origin[0], point[0]], [origin[1], point[1]], [origin[2], point[2]], c='b')

    point_prev, point_curr = point, point
    point_prev_uv, point_curr_uv = xyz_to_uv(point_prev), xyz_to_uv(point_curr)
    vel = xyz_to_uv(vec)

    #ray_color = np.array([1, 0, 0])

    for i in range(3):
        point_prev = point_curr
        point_prev_uv = point_curr_uv

        #point_curr_uv = xyz_to_uv(point_curr)

        point_curr_uv, vel = next_point(point_curr_uv, vel, 0.1)

        point_curr = uv_to_xyz(point_curr_uv)
        #ray_color[0] -= 0.2

        if i == 0:
            ray_color = np.array([0.8, 0, 0])
        elif i == 1:
            ray_color = np.array([0.8, 0.4, 0])
        elif i == 2:
            ray_color = np.array([0.8, 0.8, 0])

        ax.plot([point_prev[0], point_curr[0]], [point_prev[1], point_curr[1]], [point_prev[2], point_curr[2]], c=ray_color)


ax.set_xlabel('X')
ax.set_ylabel('Y')

plt.show()

