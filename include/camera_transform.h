#pragma once

#include "mutil.h"

class camera_transform {

    mutil::Vec3f pos {0,0,0};
    mutil::Vec3f u {1,0,0};
    mutil::Vec3f v {0,1,0};
    mutil::Vec3f n {0,0,1};


public:

    camera_transform() {}

    void setPos(const mutil::Vec3f &v);
    void handle(unsigned char key, int x, int y);
    void handle(int key, int x, int y);


    mutil::Mat4f get();


};
