#ifndef QUATERNION_H
#define QUATERNION_H

#include "vec3D.h"
#include "vec4.h"
#include "matrix.h"

class Quaternion{
public:
    float a;
    Vec3D b;
	Quaternion(float a, Vec3D b);
    void scalarMultiply(float a);
    void normalize();
    Quaternion add(Quaternion q2);
    Quaternion multiply(Quaternion q2);
    static Quaternion vectorsToQuaternion(Vec3D v1, Vec3D v2);
    Matrix toMatrix();
    
};


#endif