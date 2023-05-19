import os
import numpy as np

scene1 = '''Camera (0 5 0) (0 0 0)
#Sphere (0 0 1) (0 0 0) (4 4 4) 0 0 1 checkerboard normalmap-none (8 250 107)
Plane (0 0 3) (0 0 0) (1 1 1) 0 0 1 checkerboard normalmap-none (200 200 200)
Sphere ('''

scene2 = ''') (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (255 0 0)
Light (0 20 0) (255 140 30) 100.0
Light (0 -30 0) (255 140 30) 100.0
Light (20 0 0) (255 140 30) 100.0
Light (-20 0 0) (255 140 30) 100.0
Light (0 0 20) (255 140 30) 100.0
Light (0 0 -20) (255 140 30) 100.0'''

#spherePosition = np.array([-1.1, 10.95, 1])
spherePosition = np.array([0, 5, 0])

os.system('make')

for i in range(0, 360, 8):
    print("---------- [ ", i, " / 360 ] ----------")

    #phi = i * 3.14159265358979323846 / 180.0
    #rotationMatrix = np.array([[np.cos(phi), -np.sin(phi), 0], [np.sin(phi), np.cos(phi), 0], [0, 0, 1]])
    #rotationMatrix = np.array([[1, 0, 0], [0, np.cos(phi), -np.sin(phi)], [0, np.sin(phi), np.cos(phi)]])

   #newSpherePosition = np.matmul(rotationMatrix, spherePosition)

    newSpherePosition = np.copy(spherePosition)
    newSpherePosition[2] += i / 10.0
    spherePosition = np.copy(newSpherePosition)

    scene = scene1 + str(newSpherePosition[0]) + ' ' + str(newSpherePosition[1]) + ' ' + str(newSpherePosition[2]) + scene2

    with open('scenes/scene_anim.txt', 'w') as f:
        f.write(scene)

    os.system('./Raytracing.out ./scenes/scene_anim.txt --save anim_rotation_12/anim' + str(i) + '.png')
    os.system('rm ./scenes/scene_anim.txt')