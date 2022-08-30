#include <GL/glut.h>
#include "camera_transform.h"

using mutil::Mat4f, mutil::Vec3f;

void camera_transform::setPos(const Vec3f &v) {
    pos.x = v.x;
    pos.y = v.y;
    pos.z = v.z;
}

Mat4f camera_transform::get() {
    Mat4f ret {
	u.x,	u.y,	u.z,	-pos.x,
	v.x,	v.y,	v.z,	-pos.y,
	n.x,	n.y,	n.z,	-pos.z,
	0,	0,	0,	1
    };

    return ret;
}

void camera_transform::handle(unsigned char key, int x, int y) {
    switch (key) {
	case 'w':
	    pos.z += 0.1;
	break;
	case 's':
	    pos.z -= 0.1;
	break;
    }
}

void camera_transform::handle(int key, int x, int y) {
    switch (key) {
	case GLUT_KEY_UP:
	    pos.y += 0.06;
	break; 
	case GLUT_KEY_DOWN:
	    pos.y -= 0.06;
	break; 
	case GLUT_KEY_RIGHT:
	    pos.x += 0.06;
	break; 
	case GLUT_KEY_LEFT:
	    pos.x -= 0.06;
	break; 
    }
}

