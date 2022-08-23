#ifndef MUTIL_H
#define MUTIL_H

#include <utility>
#include <memory>

namespace mutil {

using pint = std::pair<int,int>;
extern int mmult(const pint &dima, const pint &dimb, 
		 float *a, float *b, float *res);

struct Mat4f {
    float a00,  a01,  a02,  a03,
          a10,  a11,  a12,  a13,
          a20,  a21,  a22,  a23,
          a30,  a31,  a32,  a33;

    Mat4f() {}

    Mat4f(float a00, float a01, float a02, float a03,
	  float a10, float a11, float a12, float a13,
	  float a20, float a21, float a22, float a23,
	  float a30, float a31, float a32, float a33)
    :a00(a00), a01(a01), a02(a02), a03(a03),
     a10(a10), a11(a11), a12(a12), a13(a13),
     a20(a20), a21(a21), a22(a22), a23(a23),
     a30(a30), a31(a31), a32(a32), a33(a33) {}

    float* operator[](int i);


};

extern Mat4f operator*(Mat4f a, Mat4f b);




} // namespace mutil

#endif
