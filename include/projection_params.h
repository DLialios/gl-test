#pragma once

#include <math.h>

struct projectionParams {
    float fov	= M_PI / 2;
    float nearZ	= 1;
    float farZ	= 6;
    int width	= 800;
    int height	= 600;

    projectionParams() {}

};
