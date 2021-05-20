#include "matrix.h"

Matrix::Matrix(Vec4 inp_v1, Vec4 inp_v2, Vec4 inp_v3, Vec4 inp_v4){
    v1 = inp_v1;
    v2 = inp_v2;
    v3 = inp_v3;
    v4 = inp_v4;
}

Matrix::Matrix(){};
// void Matrix::addParams(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4){
//     v1 = v1;
//     v2 = v2;
//     v3 = v3;
//     v4 = v4;
// }