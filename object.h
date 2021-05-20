#ifndef OBJECT_H
#define OBJECT_H

class Object {
public:
	Object(float x, float y, float z, float r);

	float pos[3];
	float radius;

	bool sphereCol(float d0, float d1, float d2, float ori0, float ori1, float ori2);
};

#endif