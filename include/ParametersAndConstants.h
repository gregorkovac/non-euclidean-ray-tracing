#ifndef PARAMETERSANDCONSTANTS_H
#define PARAMETERSANDCONSTANTS_H

#include "Misc.h"

// Window parameters
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

// Image plane parameters
#define IMAGE_PLANE_WIDTH 200
#define IMAGE_PLANE_HEIGHT 200

#define FOCAL_LENGTH 1

#define PIXEL_SIZE 0.01

// Ray tracing parameters
#define MAX_ITER 300
#define STEP_SIZE 0.1
#define MAX_DEPTH 2

// Newton's method parameters
#define EPSILON 0.001

// Light and color parameters
#define BRIGHTNESS 1

const Color DEFAULT_OBJECT_COLOR = {200, 200, 200};
const Color SKY_COLOR = {255, 255, 255};

// Debug parameters
#define DRAW_GIZMOS true

#endif