#include "terrain.h"
#include "vec3D.h"
#include "quaternion.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <cstdlib>

Quaternion::Quaternion(float inp_a, Vec3D inp_b){
    a = inp_a;
    b = inp_b;
}

void Quaternion::scalarMultiply(float c){
    a = c *a;
    b.mul(c);
}

void Quaternion::normalize(){
    float magnitude = sqrt(a*a + b.x*b.x + b.y*b.y + b.z*b.z);
    a = a/magnitude;
    b.mul(1/magnitude);
}

Quaternion Quaternion::add(Quaternion q2){
    Vec3D newVec = Vec3D(b.x + q2.b.x, b.y + q2.b.y, b.z + q2.b.z);
    return Quaternion(a+q2.a, Vec3D(newVec));
}

Quaternion Quaternion::multiply(Quaternion q2){    
    float newA = a*q2.a - b.x*q2.b.x - b.y*q2.b.y - b.z*q2.b.z;
    float newX = a*q2.b.x + b.x*q2.a + b.y*q2.b.z - b.z*q2.b.y;
    float newY = a*q2.b.y - b.x*q2.b.z + b.y*q2.a + b.z*q2.b.x;
    float newZ = a*q2.b.z + b.x*q2.b.y - b.y*q2.b.x + b.z*q2.a;
    Vec3D newVec = Vec3D(newX, newY, newZ);
    return Quaternion(newA, newVec);
}

Quaternion Quaternion::vectorsToQuaternion(Vec3D v1, Vec3D v2){
    Vec3D v = v2.crossProduct(v1);
    v.normalize();
    float dotProduct = v1.dotProduct(v2);
    float v1Mag = v1.getMag();
    float v2Mag = v2.getMag();
    float theta = acos(dotProduct/(v1Mag*v2Mag));
    v.mul(sin(theta/2));

    Quaternion q = Quaternion(cos(theta/2), v);
    q.normalize();

    return q;
}

Matrix Quaternion::toMatrix(){
    float qx = b.x;
    float qy = b.y;
    float qz = b.z;
    float w = a;

    Vec4 v1 = Vec4(1-2*qy*qy-2*qz*qz, 2*qx*qy-2*w*qz, 2*qx*qz+2*w*qy, 0);
    Vec4 v2 = Vec4(2*qx*qy+2*w*qz, 1-2*qx*qx-2*qz*qz, 2*qy*qz-2*w*qx, 0);
    Vec4 v3 = Vec4(2*qx*qz-2*w*qy, 2*qy*qz+2*w*qx, 1-2*qx*qx-2*qy*qy, 0);
    Vec4 v4 = Vec4(0,0,0,1);
    
    return Matrix(v1,v2,v3,v4);
}