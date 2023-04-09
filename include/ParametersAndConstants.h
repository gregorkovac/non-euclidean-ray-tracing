#ifndef PARAMETERSANDCONSTANTS_H
#define PARAMETERSANDCONSTANTS_H

#include "Misc.h"
#include "Vector.h"

// Window parameters
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// Camera parameters
#define IMAGE_PLANE_WIDTH 200
#define IMAGE_PLANE_HEIGHT 200
#define FOCAL_LENGTH 1
#define PIXEL_SIZE 0.02
#define PROJECTION 0 /* 0 ... perspective, 1 ... orthographic */

// Ray tracing parameters
#define MAX_ITER 1000
#define STEP_SIZE 0.01
#define MAX_DEPTH 4
#define RAYS_PER_PIXEL 1
#define SHADOW_RAY_COUNT 5

// Newton's method parameters
#define EPSILON 0.01

// Light and color parameters
#define BRIGHTNESS 1
#define AMBIENT_LIGHT_INTENSITY 0.1
const Color AMBIENT_LIGHT_COLOR = {255, 255, 255};

#define USE_DIRECTIONAL_LIGHT true
const Color DIRECTIONAL_LIGHT_COLOR = {255, 140, 30};
#define DIRECTIONAL_LIGHT_INTENSITY 0.02
#define DIRECTIONAL_LIGHT_DIRECTION_X -1
#define DIRECTIONAL_LIGHT_DIRECTION_Y -1
#define DIRECTIONAL_LIGHT_DIRECTION_Z 1

const Color DEFAULT_OBJECT_COLOR = {200, 200, 200};
const Color SKY_COLOR = {0, 0, 0};
const Color MISSING_COLOR = {250, 70, 250};

#define AIR_REFRACTIVE_INDEX 1

// Debug parameters
#define DRAW_GIZMOS false
#define PRINT_OBJECTS_ON_STARTUP false

#endif