#include "vec3D.h"
#include <math.h>

Vec3D::Vec3D(){};

Vec3D::Vec3D(float inx, float iny, float inz)
{
	x = inx;
	y = iny;
	z = inz;
}

float Vec3D::getX()
{
	return x;
}

float Vec3D::getY()
{
	return y;
}

float Vec3D::getZ()
{
	return z;
}

void Vec3D::setX(float tx)
{
	x = tx;
}

void Vec3D::setY(float ty)
{
	y = ty;
}

void Vec3D::setZ(float tz)
{
	z = tz;
}

void Vec3D::add(float mx, float my, float mz)
{
	x = x + mx;
	y = y + my;
	z = z + mz;
}

void Vec3D::sub(float mx, float my, float mz)
{
	x = x - mx;
	y = y - my;
	z = z - mz;
}

void Vec3D::div(float t)
{
	x = x/t;
	y = y/t;
	z = z/t;
}

void Vec3D::mul(float t)
{
	x = x*t;
	y = y*t;
	z = z*t;
}

void Vec3D::limit(float t)
{
	if (getMag() > t)
	{
		setMag(t);
	}

}

float Vec3D::getMag()
{
	return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}

float Vec3D::setMag(float t)
{
	normalize();
	mul(t);
}

float Vec3D::distance(Vec3D v1)
{
	return sqrt(pow(x-v1.getX(),2) + pow(y-v1.getY(),2) + pow(z-v1.getZ(),2));
}

void Vec3D::normalize()
{
	float norm;
	norm = (pow(x,2) + pow(y,2) + pow(z,2));
	norm = sqrt(norm);

	x = x/norm;
	y = y/norm;
	z = z/norm;
}

Vec3D Vec3D::createVector(Vec3D p1, Vec3D p2){
	return Vec3D(p2.x-p1.x, p2.y-p1.y, p2.z-p1.z);
}

Vec3D Vec3D::crossProduct(Vec3D v2){
	float cx = y*v2.z - z*v2.y;
	float cy = z*v2.x - x*v2.z;
	float cz = x*v2.y - y*v2.x;
	return Vec3D(cx,cy,cz);
}

float Vec3D::dotProduct(Vec3D v2){
	return x*v2.x + y*v2.y + z*v2.z;
}

