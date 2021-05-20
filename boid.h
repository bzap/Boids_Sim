#ifndef BOID_H
#define BOID_H

#include "vec3D.h"
#include "object.h"
#include <iostream>
#include <vector>
using namespace std;

class Boid {
public:
	Vec3D pos = {0,0,0};
	Vec3D vel = {0,0,0};
	Vec3D acc = {0,0,0};

	Boid();

	void update();
	void flock(vector<Boid> boids, int id, vector<Object> objects, vector < vector <Vec3D> > normals, float maxHeight);
	Vec3D align(vector<Boid> boids, int id);
	Vec3D cohesion(vector<Boid> boids, int id);
	Vec3D seperation(vector<Boid> boids, int id);
	Vec3D collisionAvoidance(vector<Object> objects);
	Vec3D terrainAvoidance(vector < vector <Vec3D> > normals);
	void bound();

	float maxForceAl;
	float maxForceCo;
	float maxForceSe;
	float maxForceAv;
	float getMaxForce(int type);
	void setMaxForce(int type, float f);

	float maxPercAl;
	float maxPercCo;
	float maxPercSe;
	float maxPercAv;
	float getMaxPerc(int type);
	void setMaxPerc(int type, float per);

	float maxSpeed;
	float getMaxSpeed();
	void setMaxSpeed(float speed);


private:
	float distance(float x0, float x1, float y0, float y1, float z0, float z1);
	bool lineSphereIntersection(Vec3D vec1, Vec3D vec2,Vec3D vec3, Object sphere);
	int findClosestObject(vector<Object> objects, Vec3D ahead, Vec3D ahead2, Vec3D position);
	float dotProd(Vec3D a,Vec3D b );

};


#endif