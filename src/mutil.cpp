#include "mutil.h"

namespace mutil {

using pint = std::pair<int,int>;

int mmult(const pint &dima, const pint &dimb, float *a, float *b, float *res) {
    if (dima.second != dimb.first) {
	return -1;
    }

    for (int i = 0; i < dima.first; ++i) {
	for (int j = 0; j < dimb.second; ++j) {
	    float sum = 0;
	    for (int k = 0; k < dima.second; ++k) {
		sum += a[i*dima.second+k] * b[k*dimb.second+j];
	    }
	    res[i*dimb.second+j] = sum;
	}
    }

    return 1;
}


Mat4f operator*(Mat4f a, Mat4f b) {
    Mat4f res;
    std::pair dim(4,4);
    float *ptr_a = reinterpret_cast<float*>(&a);
    float *ptr_b = reinterpret_cast<float*>(&b);
    float *ptr_res = reinterpret_cast<float*>(&res);

    mmult(dim,dim,ptr_a,ptr_b,ptr_res);

    return res; 
}

float* Mat4f::operator[](int i) {
    float *cast = reinterpret_cast<float*>(this);
    return cast + (i * 4);
}

float& Vec3f::operator[](int i) {
    switch (i) {
	case 0:
	    return x;
	case 1:
	    return y;
    }
    return z;
}

Vert::Vert(float x, float y, float z) :pos{x,y,z} {
    color.x = rand() / static_cast<float>(RAND_MAX);
    color.y = rand() / static_cast<float>(RAND_MAX);
    color.z = rand() / static_cast<float>(RAND_MAX);
}

}



