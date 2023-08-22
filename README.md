# **Non-Euclidean Ray Tracing** (Diploma thesis)

For my diploma thesis at the Faculty of Computer and Information Science of University of Ljubljana I implemented a ray tracing engine that also supports two non-euclidean spaces: **flat torus** and **spherical space**. As the focus of this project was the algorithm and experimentation, it was developed to run on the CPU instead of the GPU, therefore generating an image might take a while.

My thesis, that also explains all the details behind this project, is available [here](https://repozitorij.uni-lj.si/IzpisGradiva.php?id=148394&lang=slv). It is written in Slovenian.

## Examples
This is a comparison between the same animated scene rendered in euclidean and spherical geometry (**this animation may take some time to load on GitHub.** Alternatively you can view it in [this YouTube video](https://youtu.be/5t1IR-IGBS4)):
![AnimEuclidean](./comparison.gif)

This is the ray tracer with good settings (it takes very long to render an image like this):

![Render1](./renders/render1_merged.jpeg)

## Running the code
- `make` in the root directory
- `./Raytracing.out ./scenes/<scene_name>.txt`
  - If you also want to save the image, you can add the flag `--save` at the end

## Important files and folders
- `include/ParametersAndConstants.h` ... contains all settings of the ray tracer.
- `scenes/` ... contains scenes defined in text files. See the next chapter if you want to create your own scene.
- `other/animate_moving_sphere.py` ... an example on how to create an animation with this ray tracer using a simple Python script.
- `renders/` ... more examples of renders

## Scene definition guide
You can define your own scenes in text files with the following format:
```
Camera (Tx Ty Tz) (Rx Ry Rz)
Light (Tx Ty Tz) (Cx Cy Cz)
Object (Tx Ty Tz) (Rx Ry Rz) (Sx Sy Sz) reflectivity translucency refractiveIndex color_type normalmap-none (Cx Cy Cz)
```
- `(Tx Ty Tz)` ... translation
- `(Rx Ry Rz)` ... rotation
- `(Sx Sy Sz)` ... scale
- `color_type ` can be `solid`, `checkerboard` or `gradient`
- `(Cx Cy Cz)` ... color in RGB (0 - 255)
- You should leave `normalmap_none` as it is; normal maps are a work in progress feature.
  
The scene can only have one camera, but you can define multiple objects and lights. 

The *stb* library was used for saving the final render. It can be found [here](https://github.com/nothings/stb).
