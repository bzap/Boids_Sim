#ifndef MATRIX_H
#define MATRIX_H

#include "vec4.h"

class Matrix{
public:
    Vec4 v1,v2,v3,v4;
    Matrix();
    Matrix(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4);
    // void addParams(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4);
};


#endif