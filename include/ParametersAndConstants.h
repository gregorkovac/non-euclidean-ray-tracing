#ifndef PARAMETERSANDCONSTANTS_H
#define PARAMETERSANDCONSTANTS_H

#include "Misc.h"
#include "Vector.h"

// Window parameters
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

// Camera parameters
#define IMAGE_PLANE_WIDTH 100
#define IMAGE_PLANE_HEIGHT 100
#define FOCAL_LENGTH 1
#define PIXEL_SIZE 0.02
#define PROJECTION 0 /* 0 ... perspective, 1 ... orthographic */

// Ray tracing parameters
#define MAX_ITER 300
#define STEP_SIZE 0.01
#define MAX_DEPTH 1
#define RAYS_PER_PIXEL 1
#define SHADOW_RAY_COUNT 0

// Newton's method parameters
#define EPSILON 0.01

// Light and color parameters
#define BRIGHTNESS 1
#define AMBIENT_LIGHT_INTENSITY 0.1
const Color AMBIENT_LIGHT_COLOR = {255, 255, 255};

#define USE_DIRECTIONAL_LIGHT true
const Color DIRECTIONAL_LIGHT_COLOR = {255, 140, 30};
#define DIRECTIONAL_LIGHT_INTENSITY 0.01
#define DIRECTIONAL_LIGHT_DIRECTION_X -1
#define DIRECTIONAL_LIGHT_DIRECTION_Y -1
#define DIRECTIONAL_LIGHT_DIRECTION_Z 1

const Color BLACK = {0, 0, 0};
const Color DEFAULT_OBJECT_COLOR = {200, 200, 200};
const Color SKY_COLOR = {255, 255, 255};
const Color MISSING_COLOR = {250, 70, 250};

#define AIR_REFRACTIVE_INDEX 1

// Math constants
#define PI 3.14159265358979323846

// Debug parameters
#define DRAW_GIZMOS false
#define PRINT_OBJECTS_ON_STARTUP false

#endif