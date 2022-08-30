#pragma once

#include "mutil.h"

enum class wTrans {
    translate,
    rotatex,
    rotatey,
    rotatez,
    scale
};

class world_transform {
    mutil::Mat4f m {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
    };

    mutil::Mat4f t = m, 
		 rx = m, 
	         ry = m, 
		 rz = m, 
		 s = m;

    wTrans order[5] {
	wTrans::scale,
	wTrans::rotatex,
	wTrans::rotatey,
	wTrans::rotatez,
	wTrans::translate
    };

public:
    world_transform() {}

    void setTranslate(const mutil::Vec3f &v);
    void setRotate(const mutil::Vec3f &v);
    void setScale(const mutil::Vec3f &v);
    void setOrder(wTrans a,
		  wTrans b,
		  wTrans c,
		  wTrans d,
		  wTrans e);

    mutil::Mat4f get();
};
