#include "boid.h"
#include "object.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

Boid::Boid()
{
	float xRand = (rand() % 90) - 40;
	float yRand = (rand() % 90) - 40;
	float zRand = (rand() % 90) - 40;

	pos.setX(xRand);
	pos.setY(yRand);
	pos.setZ(zRand);

	vel.setX(rand());
	vel.setY(rand());
	vel.setZ(rand());
	vel.normalize();

	acc.setX(0);
	acc.setY(0);
	acc.setZ(0);

	maxForceAl = 0.08;
	maxForceCo = 0.01;
	maxForceSe = 0.05;
	maxForceAv = 0.4;

	maxPercAl = 7;
	maxPercCo = 10;
	maxPercSe = 5;
	maxPercAv = 10;

	maxSpeed = 1;
}

Vec3D Boid::align(vector<Boid> boids, int id)
{
	Vec3D avg(0,0,0);
	int total = 0;

	for(int i = 0; i < boids.size(); i++)
	{
		if (pos.distance(boids[i].pos) <= maxPercAl && i != id)
		{
			avg.add(boids[i].vel.getX(),boids[i].vel.getY(),boids[i].vel.getZ());
			total = total + 1;
		}
	}

	if (total > 0)
	{
		avg.div(total);
		avg.setMag(maxSpeed);
		avg.sub(vel.getX(),vel.getY(),vel.getZ());
		avg.limit(maxForceAl);
		
	}

	return avg;


}

Vec3D Boid::cohesion(vector<Boid> boids, int id)
{
	Vec3D avg(0,0,0);
	int total = 0;

	for(int i = 0; i < boids.size(); i++)
	{
		if (pos.distance(boids[i].pos) <= maxPercCo && i != id)
		{
			avg.add(boids[i].pos.getX(),boids[i].pos.getY(),boids[i].pos.getZ());
			total = total + 1;
		}
	}

	if (total > 0)
	{
		avg.div(total);
		avg.sub(pos.getX(),pos.getY(),pos.getZ());
		avg.setMag(maxSpeed);
		avg.sub(vel.getX(),vel.getY(),vel.getZ());
		avg.limit(maxForceCo);
		
	}

	return avg;

}

Vec3D Boid::seperation(vector<Boid> boids, int id)
{
	Vec3D avg(0,0,0);
	int total = 0;

	for(int i = 0; i < boids.size(); i++)
	{
		float dist = pos.distance(boids[i].pos);
		if (dist <= maxPercSe && i != id)
		{
			Vec3D diff(pos.getX() - boids[i].pos.getX(), pos.getY() - boids[i].pos.getY(), pos.getZ() - boids[i].pos.getZ());
			diff.div(dist);
			avg.add(diff.getX(),diff.getY(),diff.getZ());
			total = total + 1;
		}
	}

	if (total > 0)
	{
		avg.div(total);
		avg.setMag(maxSpeed);
		avg.sub(vel.getX(),vel.getY(),vel.getZ());
		avg.limit(maxForceSe);
		
	}

	return avg;
}

Vec3D Boid::collisionAvoidance(vector<Object> objects)
{
	Vec3D normVel = {vel.getX(), vel.getY(), vel.getZ()};
	normVel.normalize();

	float dynamic_mag = vel.getMag()/maxSpeed;

	Vec3D ahead = {pos.getX() + (normVel.getX()*dynamic_mag),
				   pos.getY() + (normVel.getY()*dynamic_mag),
				   pos.getZ() + (normVel.getZ()*dynamic_mag) };

	Vec3D ahead2 = {pos.getX() + (normVel.getX()*dynamic_mag*0.5),
				    pos.getY() + (normVel.getY()*dynamic_mag*0.5),
				    pos.getZ() + (normVel.getZ()*dynamic_mag*0.5) };

	int mostThreat = findClosestObject(objects, ahead, ahead2, pos);
	Vec3D avoidance = {0,0,0};

	if(mostThreat != -1)
	{
		avoidance.setX(ahead.getX() - objects[mostThreat].pos[0]);
		avoidance.setY(ahead.getY() - objects[mostThreat].pos[1]);
		avoidance.setZ(ahead.getZ() - objects[mostThreat].pos[2]);

		avoidance.normalize();
		avoidance.mul(maxForceAv);
	}
	//cout << mostThreat << endl;
	return avoidance;
}

int Boid::findClosestObject(vector<Object> objects, Vec3D ahead, Vec3D ahead2, Vec3D position)
{
	if(objects.size() > 0)
	{
		int mostThreat = -1;

		for(int i = 0; i < objects.size(); i++)
		{
			bool col = lineSphereIntersection(ahead,ahead2, position, objects[i]);

			float distObj = distance(objects[i].pos[0], pos.getX(),
									 objects[i].pos[1], pos.getY(),
									 objects[i].pos[2], pos.getZ());

			float distThreat;
			if(mostThreat != -1)
			{
				distThreat = distance(objects[mostThreat].pos[0], pos.getX(),
										 objects[mostThreat].pos[1], pos.getY(),
										 objects[mostThreat].pos[2], pos.getZ());
			}
			else
			{
				distThreat = 10000;
			}

			if(col == true && distObj < distThreat)
			{
				mostThreat = i;
			}
		}

		//cout << mostThreat << endl;
		return mostThreat;
	}

	return -1;
}

float Boid::distance(float x0, float x1, float y0, float y1, float z0, float z1)
{
	return sqrt(pow(x0-x1,2) + pow(y0-y1,2) + pow(z0-z1,2));
}

bool Boid::lineSphereIntersection(Vec3D vec1, Vec3D vec2,Vec3D vec3, Object sphere)
{
	return (distance(sphere.pos[0], vec1.getX(), sphere.pos[1], vec1.getY(), sphere.pos[2], vec1.getZ()) <= sphere.radius ||
			distance(sphere.pos[0], vec2.getX(), sphere.pos[1], vec2.getY(), sphere.pos[2], vec2.getZ()) <= sphere.radius ||
			distance(sphere.pos[0], vec3.getX(), sphere.pos[1], vec3.getY(), sphere.pos[2], vec3.getZ()) <= sphere.radius);
}

float Boid::dotProd(Vec3D a,Vec3D b )
{
	float temp = a.getX()*b.getX() + 
				  a.getY()*b.getY()+
				  a.getZ()*b.getZ();
	return temp;
}

Vec3D Boid::terrainAvoidance(vector < vector <Vec3D> > normals)
{
	int currX = pos.getX() + 48;
	int currZ = pos.getZ() + 48;

	Vec3D nul = {0,0,0};

	if(currX > 0 &&  currX < 98 && currZ < 0 && currZ < 98)
	{
		Vec3D tampVel = {vel.getX(), vel.getY(), vel.getZ()};
		Vec3D tampG = {normals[currX][currZ].getX(),normals[currX][currZ].getY(), normals[currX][currZ].getZ()};

		float sv = dotProd(tampVel,tampG);

		if(tampVel.getMag() > 0.0001 && sqrt(pow(sv,2)+1) > 0.0001)
		{	
			sv = sv/tampVel.getMag();
			tampVel.div(sqrt(pow(sv,2)+1));
		}
		else { return nul;}

		return tampVel;
	}


	return nul;

}

void Boid::flock(vector<Boid> boids, int id, vector<Object> objects,vector < vector <Vec3D> > normals, float maxHeight)
{
	acc.add(align(boids,id).getX(),align(boids,id).getY(),align(boids,id).getZ());
	acc.add(cohesion(boids,id).getX(),cohesion(boids,id).getY(),cohesion(boids,id).getZ());
	acc.add(seperation(boids,id).getX(),seperation(boids,id).getY(),seperation(boids,id).getZ());
	acc.add(collisionAvoidance(objects).getX(),collisionAvoidance(objects).getY(),collisionAvoidance(objects).getZ());

	if(pos.getY() < maxHeight)
	{
		acc.add(terrainAvoidance(normals).getX(), terrainAvoidance(normals).getY(), terrainAvoidance(normals).getZ());
	}
}

void Boid::update()
{
	pos.add(vel.getX(),vel.getY(),vel.getZ());
	vel.add(acc.getX(),acc.getY(),acc.getZ());
	vel.limit(maxSpeed);
	acc.mul(0);
}

void Boid::bound()
{
	//Stays in bound
	if(pos.getX() <= -40)
	{
		vel.add(0.1*maxSpeed,0,0);
	}
	if(pos.getX() >= 40)
	{
		vel.add(-0.1*maxSpeed,0,0);
	}
	if(pos.getY() <= -40)
	{
		vel.add(0,0.1*maxSpeed,0);
	}
	if(pos.getY() >= 40)
	{
		vel.add(0,-0.1*maxSpeed,0);
	}
	if(pos.getZ() <= -40)
	{
		vel.add(0,0,0.1*maxSpeed);
	}
	if(pos.getZ() >= 40)
	{
		vel.add(0,0,-0.1*maxSpeed);
	}
}

float Boid::getMaxForce(int type)
{
	if(type == 0)
	{
		return maxForceAl;
	}
	else if(type == 1)
	{
		return maxForceCo;
	}
	else
	{
		return maxForceSe;
	}
}

void Boid::setMaxForce(int type, float f)
{
	if(type == 0)
	{
		maxForceAl = f;
	}
	else if(type == 1)
	{
		maxForceCo = f;
	}
	else
	{
		maxForceSe = f;
	}
}

float Boid::getMaxPerc(int type)
{
	if(type == 0)
	{
		return maxPercAl;
	}
	else if(type == 1)
	{
		return maxPercCo;
	}
	else
	{
		return maxPercSe;
	}
}

void Boid::setMaxPerc(int type, float per)
{
	if(type == 0)
	{
		maxPercAl = per;
	}
	else if(type == 1)
	{
		maxPercCo = per;
	}
	else
	{
		maxPercSe = per;
	}
}

float Boid::getMaxSpeed()
{
	return maxSpeed;
}

void Boid::setMaxSpeed(float speed)
{
	maxSpeed = speed;
}
