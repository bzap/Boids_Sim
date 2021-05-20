#include "object.h"

Object::Object(float x, float y, float z, float r)
{
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;

	radius = r;
}

bool Object::sphereCol(float d0, float d1, float d2, float ori0, float ori1, float ori2)
{
	float diff[3] = {ori0 - pos[0], ori1-pos[1], ori2-pos[2]};

	float A = (d0 * d0) + (d1 * d1) + (d2 * d2);
	float B = 2 * ((diff[0]*d0) + (diff[1]*d1) + (diff[2]*d2));
	float C = ((diff[0]*diff[0]) + (diff[1]*diff[1]) + (diff[2]*diff[2])) - (radius*radius);

	float discrim = (B*B) - (4*A*C);

	if(discrim > 0)
	{
		return true;
	}

	return false;
}