import os
import numpy as np


#scene1 = '''
#Camera (0 11 0) (0 0 0)
#Sphere (0 0 0) (0 0 0) (10 10 10) 0 0 1 checkerboard normalmap-none (0 148 50)
#Sphere ('''
#
#scene2 = ''') (0 0 0) (1 1 1) 0.4 0 1 solid normalmap-none (251 197 49)
#Sphere (3 11 2) (0 0 0) (1 1 1) 0 0 1 gradient normalmap-none (234 32 39)
#Sphere (-4 10 6) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (18 203 196)
#Sphere (4 9 8) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (153 128 250)
#Sphere (-3 -11 2) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (255 168 1)
#Light (0 15 0) (255 255 255) 1.0
##Light (0 20 5) (255 140 30) 100.0
##Light (0 20 10) (255 140 30) 100.0
##Light (0 20 20) (255 140 30) 100.0
##Light (0 0 20) (255 140 30) 100.0
##Light (20 0 0) (255 140 30) 100.0'''

scene1 = '''
Camera (0 11 0) (0 0 0)
Sphere (0 0 0) (0 0 0) (10 10 10) 0 0 1 solid normalmap-none (46 204 113)
Sphere ('''

scene2 = ''') (0 0 0) (1 1 1) 0.4 0 1 solid normalmap-none (251 197 49)
Sphere (3 11 2) (0 0 0) (1 1 1) 0 0 1 gradient normalmap-none (234 32 39)
Sphere (-4 10 6) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (18 203 196)
Sphere (4 9 8) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (153 128 250)
Sphere (-3 -11 2) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (255 168 1)
Light (0 13 0) (255 255 255) 3.0
Light (0 13 4) (255 255 255) 3.0
Light (0 11 6) (255 255 255) 3.0
'''


#spherePosition = np.array([-1.1, 10.95, 1])
spherePosition = np.array([0, 11, 0])

os.system('make')

for i in range(12, 1000, 1):
    print("---------- [ ", i, " / 1000 ] ----------")

    phi = (i / 2.0) * 3.14159265358979323846 / 180.0
    #rotationMatrix = np.array([[np.cos(phi), -np.sin(phi), 0], [np.sin(phi), np.cos(phi), 0], [0, 0, 1]])
    rotationMatrix = np.array([[1, 0, 0], [0, np.cos(phi), -np.sin(phi)], [0, np.sin(phi), np.cos(phi)]])

    newSpherePosition = np.matmul(rotationMatrix, spherePosition)

    scene = scene1 + str(newSpherePosition[0]) + ' ' + str(newSpherePosition[1]) + ' ' + str(newSpherePosition[2]) + scene2

    with open('scenes/scene_anim.txt', 'w') as f:
        f.write(scene)

    os.system('./Raytracing.out ./scenes/scene_anim.txt --save anim_diploma_euclidean/anim' + str(i) + '.png')
    os.system('rm ./scenes/scene_anim.txt')