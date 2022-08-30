#include <math.h>
#include "perspective_transform.h"
#include "mutil.h"

using mutil::Mat4f;

Mat4f perspective_transform::get() {
    float tanHalfFOV = tanf(ptr->fov / 2);
    float d = 1 / tanHalfFOV;
    float ar = (float) ptr->width / ptr->height;
    float A = (-ptr->nearZ-ptr->farZ)/(ptr->nearZ-ptr->farZ);
    float B = (2*ptr->farZ*ptr->nearZ)/(ptr->nearZ-ptr->farZ);

    Mat4f ret {
	d/ar,0,0,0,
	0,d,0,0,
	0,0,A,B,
	0,0,1,0
    };

    return ret;
}
