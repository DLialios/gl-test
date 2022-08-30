#pragma once

#include "mutil.h"
#include "projection_params.h"

struct perspective_transform {

    const projectionParams *ptr;

    perspective_transform(const projectionParams *ptr) :ptr(ptr) {}

    mutil::Mat4f get(); 

};
