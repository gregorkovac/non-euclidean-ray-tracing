import os

scene1 = '''
# SCENE FILE FORMAT:
# Camera (Tx Ty Tz) (Rx Ry Rz)
# Light (Tx Ty Tz) (Cx Cy Cz)
# Object (Tx Ty Tz) (Rx Ry Rz) (Sx Sy Sz) reflectivity translucency refractiveIndex color_type (Cx Cy Cz)

Camera (0 0 '''

scene2 = ''') (0 0 0)
Plane (0 -1.1 0) (0 0 0) (1 1 1) 0 0 1 checkerboard normalmap-none (200 0 255)
Plane (0 0 2) (1.5708 0 0) (1 1 1) 0 0 1 checkerboard normalmap-none (0 200 255)
Light (1 1 1) (255 255 255) 2.0
'''

for i in range(-10, 10):
    camera_z = i / 10.0

    scene = scene1 + str(camera_z) + scene2

    with open('scenes/scene_anim.txt', 'w') as f:
        f.write(scene)

    os.system('./Raytracing.out ./scenes/scene_anim.txt --save anim1/anim' + str(i + 11) + '.png')
    os.system('rm ./scenes/scene_anim.txt')
