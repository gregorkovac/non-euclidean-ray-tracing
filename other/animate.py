import os

scene1 = '''Camera (3 11 2) (0 '''

scene2 = ''' 0)
Sphere (0 -5 0) (0 0 0) (15 15 15) 0 0 1 checkerboard normalmap-none (8 250 107)
Sphere (-1.1 10.95 1) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (255 0 0)
Sphere (-3.25 10.51 0.7) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (100 50 0)
Sphere (-5.27 9.65 0.4) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (150 100 0)
Sphere (-7.09 8.41 0.1) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (100 150 0)
Sphere (-8.62 6.84 -0.2) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (50 200 0)
Sphere (-9.8 4.99 -0.5) (0 0 0) (1 1 1) 0 0 1 solid normalmap-none (0 255 0)
Light (0 20 0) (255 140 30) 150.0
'''

os.system('make')

for i in range(0, 45):
    print("---------- [ ", i, " / 45 ] ----------")

    angle = i * 8
    angle = angle * 3.14159265358979323846 / 180.0

    scene = scene1 + str(angle) + scene2

    with open('scenes/scene_anim.txt', 'w') as f:
        f.write(scene)

    os.system('./Raytracing.out ./scenes/scene_anim.txt --save anim_rotation_eucl/anim' + str(i) + '.png')
    os.system('rm ./scenes/scene_anim.txt')
