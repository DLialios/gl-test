#include <math.h>
#include "world_transform.h"
#include "mutil.h"

using mutil::Mat4f, mutil::Vec3f;

void world_transform::setTranslate(const Vec3f &v) {
    t = {
	1,  0,	0,  v.x,
	0,  1,	0,  v.y,
	0,  0,	1,  v.z,
	0,  0,	0,  1
    };
}

void world_transform::setRotate(const Vec3f &v) {
    rx = {
	1,  0,		0,	    0,
	0,  cosf(v.x),	-sinf(v.x), 0,
	0,  sinf(v.x),	cosf(v.x),  0,
	0,  0,		0,	    1
    };
    ry = {
	cosf(v.y),  0,	sinf(v.y),  0,
	0,	    1,	0,	    0,
	-sinf(v.y), 0,	cosf(v.y),  0,
	0,	    0,	0,	    1
    };
    rz = {
	cosf(v.z),  -sinf(v.z),	0,  0,
	sinf(v.z),  cosf(v.z),	0,  0,
	0,	    0,		1,  0,
	0,	    0,		0,  1
    };
}


void world_transform::setScale(const Vec3f &v) {
    s = {
	v.x,	0,	0,	0,
	0,	v.y,	0,	0,
	0,	0,	v.z,	0,
	0,	0,	0,	1
    };
}

void world_transform::setOrder(wTrans a,
			       wTrans b,
			       wTrans c,
			       wTrans d,
			       wTrans e) {
    order[0] = a;
    order[1] = b;
    order[2] = c;
    order[3] = d;
    order[4] = e;
}

Mat4f world_transform::get() {
    for (int i = 0; i < 5; ++i) {
	switch (order[i]) {
	    case wTrans::translate:
		m = t * m;
	    break;
	    case wTrans::rotatex:
		m = rx * m;
	    break;
	    case wTrans::rotatey:
		m = ry * m;
	    break;
	    case wTrans::rotatez:
		m = rz * m;
	    break;
	    case wTrans::scale:
		m = s * m;
	    break;
	}
    }

    return m;
}


