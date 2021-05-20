#ifndef VEC_3D_H
#define VEC_3D_H

class Vec3D {
public:
	Vec3D();
	Vec3D(float inx, float iny, float inz);

	//Get vector values
	float getX();
	float getY();
	float getZ();

	//Set vector values
	void setX(float tx);
	void setY(float ty);
	void setZ(float tz);

	void add(float mx, float my, float mz);
	void sub(float mx, float my, float mz);
	void div(float t);
	void mul(float t);
	void normalize();

	void limit(float t);
	float getMag();
	float setMag(float t);


	float distance(Vec3D v1);
	static Vec3D createVector(Vec3D p1, Vec3D p2);
	Vec3D crossProduct(Vec3D v2);
	float dotProduct(Vec3D v2);


	float x;
	float y;
	float z;

};


#endif