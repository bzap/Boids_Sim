
#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "boid.h"
#include "vec3D.h"
#include <vector>
#include <time.h>
#include <cmath>
#include <math.h>

#include "gui.h"
#include "terrain.h"
#include "quaternion.h"
#include "matrix.h"
#include "vec4.h"

#include <iostream>
#include <fstream>
// Using the great OBJLoader library found at: https://github.com/Bly7/OBJ-Loader
// Used to load and manipulate obj files 
#include "OBJ_Loader.h"
// Using the great stb_image library found at: https://github.com/nothings/stb/blob/master/stb_image.h
// Used to load images for textures 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "object.h"
// Using the PPM loader library given in the labs 
#include "PPM.h"

GLUquadricObj * sphere;
GLuint textureList[4];
GLubyte * tex[4];

int mWidth[4];
int mHeight[4];
int n[4];

GLfloat ambient0[4] = {0.576, 0.439, 0.859, 1};
GLfloat diffuse0[4] = {0.2, 0.2, 1, 1}; 
GLfloat specular0[4] = {0.600, 0.196, 0.800, 1};
GLfloat pos0[4] = {5.0, 150.0, 1.0, 1.0};

float windowX = 1000;
float windowY = 1000; 

vector<Boid> boids;
int selectedObj = -1;
int selectedBoid = -1;

GLdouble eye[] = { 0, 30, 150 };
GLdouble lookAt[] = { 0, 0, 0 };
GLdouble up[] = { 0, 1, 0 };
GLdouble test = 500;
vector<Vec3D> pastVels;

bool trackingBoid = 0;
int trackingIndex = 0;

float pi = 3.1415926;
float yRotation = 0;
float xRotation = pi/4;
float viewDistance = 200;
int gridSize = 100;
Terrain terrain(gridSize, -50);

objl::Loader objValues;
objl::Mesh mesh0;
objl::Mesh mesh;
bool objFile; 
GLuint texture;

int numOfButtons = 16;
int buttonCord[16][3] = {{200,400},{300,400},
						{50,300},{150,300},
						{50,200},{150,200},
						{50,100},{150,100},
						{300,300},{400,300},
						{300,200},{400,200},
						{300,100},{400,100},
						{200,50},{300,50}};



Gui GUI(500,500);

int wind1;
int wind2;
int wind3;

double rayNear[3];
double rayFar[3];

bool mouseDown = 0;
float clickXPos, clickYPos;
Quaternion rotationQuaternion(-1, Vec3D(-1,-1,-1));
bool rotationMatChanged = 0;

vector<Object> objects;
vector<Object> boidSph;
bool pause = false;

//Makes the enviorment in which the simulation takes place
void generateEnvironmentWires(){
    glPushMatrix();
    	glScalef(50,50,50);
	    glBegin(GL_LINE_LOOP);
		    glVertex3f(  1.0f, 1.0f, -1.0f );
		    glVertex3f( -1.0f, 1.0f, -1.0f ); 
		    glVertex3f( -1.0f, 1.0f,  1.0f );  
		    glVertex3f(  1.0f, 1.0f,  1.0f ); 
		    glEnd();

		 	glBegin(GL_LINE_LOOP);
			    // Bottom face
			    glVertex3f(  1.0f, -1.0f, -1.0f ); 
			    glVertex3f( -1.0f, -1.0f, -1.0f ); 
			    glVertex3f( -1.0f, -1.0f,  1.0f ); 
			    glVertex3f(  1.0f, -1.0f,  1.0f ); 
		    glEnd();
		  
		  	glBegin(GL_LINE_LOOP);
			    // Front face
			    glVertex3f(  1.0f,  1.0f, 1.0f );  
			    glVertex3f( -1.0f,  1.0f, 1.0f );  
			    glVertex3f( -1.0f, -1.0f, 1.0f );  
			    glVertex3f(  1.0f, -1.0f, 1.0f );  
		    glEnd();
		 
		 	glBegin(GL_LINE_LOOP);
			    // Back face
			    glVertex3f(  1.0f, -1.0f, -1.0f ); 
			    glVertex3f( -1.0f, -1.0f, -1.0f ); 
			    glVertex3f( -1.0f,  1.0f, -1.0f ); 
			    glVertex3f(  1.0f,  1.0f, -1.0f ); 
		    glEnd();
		 
		 	glBegin(GL_LINE_LOOP);
			    // Left face
			    glVertex3f( -1.0f,  1.0f,  1.0f);  
			    glVertex3f( -1.0f,  1.0f, -1.0f);  
			    glVertex3f( -1.0f, -1.0f, -1.0f);  
			    glVertex3f( -1.0f, -1.0f,  1.0f);  
		    glEnd();
		 
		 	glBegin(GL_LINE_LOOP);
			    // Right face
			    glVertex3f(  1.0f,  1.0f,  1.0f);  
			    glVertex3f(  1.0f,  1.0f, -1.0f);  
			    glVertex3f(  1.0f, -1.0f, -1.0f);  
			    glVertex3f(  1.0f, -1.0f,  1.0f);  
	    glEnd();
    glPopMatrix();
}

//Makes the enviorment in which the simulation takes place
void generateEnvironment(){
    glPushMatrix();
    	glScalef(50,50,50);
		glColor3f(1,1,1);
		glLineWidth(3);
	    glBegin(GL_QUADS);
		    glVertex3f(  1.0f, 1.0f, -1.0f );
		    glVertex3f( -1.0f, 1.0f, -1.0f ); 
		    glVertex3f( -1.0f, 1.0f,  1.0f );  
		    glVertex3f(  1.0f, 1.0f,  1.0f ); 
		    glEnd();

		 	glBegin(GL_QUADS);
			    // Bottom face
			    glVertex3f(  1.0f, -1.0f, -1.0f ); 
			    glVertex3f( -1.0f, -1.0f, -1.0f ); 
			    glVertex3f( -1.0f, -1.0f,  1.0f ); 
			    glVertex3f(  1.0f, -1.0f,  1.0f ); 
		    glEnd();
		  
		  	glBegin(GL_QUADS);
			    // Front face
				glTexCoord2f(0, 0);
			    glVertex3f(  1.0f,  1.0f, 1.0f );
				glTexCoord2f(0, 1);
			    glVertex3f( -1.0f,  1.0f, 1.0f );  
				glTexCoord2f(1, 0);
			    glVertex3f( -1.0f, -1.0f, 1.0f );  
				glTexCoord2f(1, 1);
			    glVertex3f(  1.0f, -1.0f, 1.0f );  
		    glEnd();
		 	glBegin(GL_QUADS);
			    // Back face
			    glVertex3f(  1.0f, -1.0f, -1.0f ); 
			    glVertex3f( -1.0f, -1.0f, -1.0f ); 
			    glVertex3f( -1.0f,  1.0f, -1.0f ); 
			    glVertex3f(  1.0f,  1.0f, -1.0f ); 
		    glEnd();
		 	glBegin(GL_QUADS);
			    // Left face
			    glVertex3f( -1.0f,  1.0f,  1.0f);  
			    glVertex3f( -1.0f,  1.0f, -1.0f);  
			    glVertex3f( -1.0f, -1.0f, -1.0f);  
			    glVertex3f( -1.0f, -1.0f,  1.0f);  
		    glEnd();
		 	glBegin(GL_QUADS);
			    // Right face
				glTexCoord2f(0, 0);
			    glVertex3f(  1.0f,  1.0f,  1.0f);  
				glTexCoord2f(0, 1);
			    glVertex3f(  1.0f,  1.0f, -1.0f);  
				glTexCoord2f(1, 0);
			    glVertex3f(  1.0f, -1.0f, -1.0f);  
				glTexCoord2f(1, 1);
			    glVertex3f(  1.0f, -1.0f,  1.0f);  
	    glEnd();
    glPopMatrix();
}


void drawGrid(vector <vector <Vec3D> > grid, vector <vector <Vec3D> >normals){
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
    for(int i = 0; i < grid.size() - 1; i ++){
        glBegin(GL_QUADS);
        for (int j = 0; j < grid.size() - 1; j++){
            if (j < grid.size()-2 && i < grid.size()-2){
                glNormal3f(normals[i][j].getX(), normals[i][j].getY(), normals[i][j].getZ());
            }
            glColor3f(0.5,0.5,0.5);
			glTexCoord2f(0, 0);
			glVertex3f(grid[i][j].getX(),grid[i][j].getY(),grid[i][j].getZ());
			glTexCoord2f(0, 1);
			glVertex3f(grid[i][j + 1].getX(),grid[i][j + 1].getY(),grid[i][j + 1].getZ());
			glTexCoord2f(1, 1);
			glVertex3f(grid[i + 1][j + 1].getX(),grid[i + 1][j + 1].getY(),grid[i + 1][j + 1].getZ());
			glTexCoord2f(1, 0);
			glVertex3f(grid[i + 1][j].getX(),grid[i + 1][j].getY(),grid[i + 1][j].getZ());
        }
        glEnd();
    }
	glPopMatrix();
}

Vec3D averageVel(){
	float avgXVel,avgYVel,avgZVel;
	for(int i = 0; i < pastVels.size(); i ++){
		avgXVel += pastVels[i].getX();
		avgYVel += pastVels[i].getY();
		avgZVel += pastVels[i].getZ();
	}
	return Vec3D(avgXVel/pastVels.size(), avgYVel/pastVels.size(), avgZVel/pastVels.size());
}

// Renders and maps textures to a boid using vertices provided in an obj file 
void drawBoid(){
	if (objFile) {
		glBegin(GL_TRIANGLES);
				 for (int y = 0; y < mesh0.Vertices.size(); y++){	
					glNormal3f(mesh.Vertices[y].Normal.X, mesh.Vertices[y].Normal.Y, mesh.Vertices[y].Normal.Z);
					glTexCoord2f(mesh0.Vertices[y].TextureCoordinate.X, mesh0.Vertices[y].TextureCoordinate.Y);
					glVertex3f(mesh0.Vertices[y].Position.X, mesh0.Vertices[y].Position.Y, mesh0.Vertices[y].Position.Z);		
				} 

				for (int y = 0; y < mesh.Vertices.size(); y++){	
					glNormal3f(mesh.Vertices[y].Normal.X, mesh.Vertices[y].Normal.Y, mesh.Vertices[y].Normal.Z);
					glTexCoord2f(mesh.Vertices[y].TextureCoordinate.X, mesh.Vertices[y].TextureCoordinate.Y);
					glVertex3f(mesh.Vertices[y].Position.X, mesh.Vertices[y].Position.Y, mesh.Vertices[y].Position.Z);		
				} 
			}
		glEnd();
}

void drawObjects()
{
	for(int i = 0; i < objects.size(); i++)
	{
		glPushMatrix();
			glTranslatef(objects[i].pos[0],objects[i].pos[1],objects[i].pos[2]);
			glScalef(0.95,0.95,0.95);
			sphere = gluNewQuadric();
			gluQuadricNormals(sphere, GLU_SMOOTH);
			gluQuadricOrientation(sphere, GLU_OUTSIDE);
			gluQuadricDrawStyle(sphere, GLU_FILL);
			gluQuadricTexture(sphere, GL_TRUE);
			gluSphere(sphere, objects[i].radius-1, 10, 10); 
		glPopMatrix();
	}
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);



	if(!trackingBoid){

		float newEyeX = viewDistance * cos(yRotation) * sin(xRotation);
		float newEyeY = viewDistance * cos(xRotation);
		float newEyeZ = viewDistance * sin(yRotation) * sin(xRotation);

		gluLookAt(
			eye[0], eye[1], eye[2],
			lookAt[0], lookAt[1], lookAt[2],
			up[0], up[1], up[2]
		);
	}
	else{
		Boid trackedBoid = boids[trackingIndex];
		Vec3D past3AvgVel = averageVel();
		Vec3D boidPos = trackedBoid.pos;
		GLdouble newCameraX = boidPos.getX() + past3AvgVel.getX()/past3AvgVel.getMag();
		GLdouble newCameraY = boidPos.getY() + past3AvgVel.getY()/past3AvgVel.getMag();
		GLdouble newCameraZ = boidPos.getZ() + past3AvgVel.getZ()/past3AvgVel.getMag();

		GLdouble newLookatX = boidPos.getX() + 2*past3AvgVel.getX()/past3AvgVel.getMag();
		GLdouble newLookatY = boidPos.getY() + 2*past3AvgVel.getY()/past3AvgVel.getMag();
		GLdouble newLookatZ = boidPos.getZ() + 2*past3AvgVel.getZ()/past3AvgVel.getMag();
		gluLookAt(
			newCameraX, newCameraY, newCameraZ,
			newLookatX, newLookatY, newLookatZ,
			up[0], up[1], up[2]
		);
	}

	if(rotationQuaternion.a != -1 && !trackingBoid)
	{
		Matrix m = rotationQuaternion.toMatrix();

		GLfloat a[16] = {m.v1.c1, m.v1.c2, m.v1.c3, m.v1.c4, m.v2.c1, m.v2.c2, m.v2.c3, m.v2.c4,
					m.v3.c1, m.v3.c2, m.v3.c3, m.v3.c4, m.v4.c1, m.v4.c2, m.v4.c3, m.v4.c4};
		glMatrixMode(GL_MODELVIEW);
		glMultMatrixf(a);

	}
	
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureList[2]);
	generateEnvironment();
	glDisable(GL_TEXTURE_2D);
	generateEnvironmentWires();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureList[1]);
	drawGrid(terrain.grid, terrain.normals);
	glDisable(GL_TEXTURE_2D);

	for(int i = 0; i < boids.size(); i++ )
	{
		glPushMatrix();
		glTranslatef(boids[i].pos.getX(),boids[i].pos.getY(),boids[i].pos.getZ());
		glScalef(10, 10, 10);
		//glRotatef(180, boids[i].pos.getX(), boids[i].pos.getY(), boids[i].pos.getZ());
		//glRotatef(atan2(boids[i].pos.getZ(),boids[i].pos.getX()) * 180 / M_PI,0,1,0);
		//glRotatef(asin(boids[i].pos.getY()) * 180 / M_PI,0,0,1);
		//float mag = (sqrtf(powf(boids[i].pos.getX(), 2.0) + powf(boids[i].pos.getY(), 2.0) +  powf(boids[i].pos.getZ(), 2.0)));
		//float xcos = acos(boids[i].pos.getX() / mag) * (180/pi); 
		//float ycos = acos(boids[i].pos.getY() / mag) * (180/pi); 
		//float zcos = acos(boids[i].pos.getZ() / mag) * (180/pi); 
		//glRotatef(xcos,boids[i].pos.getX(),boids[i].pos.getY(),boids[i].pos.getZ());
		//glRotatef(ycos,boids[i].pos.getX(),boids[i].pos.getY(),boids[i].pos.getZ());
		glRotatef(45,boids[i].pos.getX(),boids[i].pos.getY(),boids[i].pos.getZ());

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureList[0]);
		drawBoid();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureList[3]);
	drawObjects();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
    glutPostRedisplay();
	glFlush();
}

void handleReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /**
     * Call gluPerspective here.
     */
    gluPerspective(45, 1, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

void FPS(int x){

	if(pause == false)
	{
		for(int i = 0; i < boids.size(); i++ )
	    {
	    	boids[i].update();
	    	boids[i].bound();
	    	boids[i].flock(boids, i, objects,terrain.normals,terrain.maxHeight);
	    	boidSph[i].pos[0] = boids[i].pos.getX();
	    	boidSph[i].pos[1] = boids[i].pos.getY();
	    	boidSph[i].pos[2] = boids[i].pos.getZ();
	    	//boids[i].cohesion(boids, i);
	    }

		//cout << b1.pos.getX();
		if (pastVels.size() < 20){
			pastVels.push_back(boids[trackingIndex].vel);
		}
		else{
			pastVels.erase(pastVels.begin()+2);
			pastVels[0] = (boids[trackingIndex].vel);
		}
	}

	glutSetWindow(wind1);
	glutPostRedisplay();
	glutTimerFunc(17,FPS,0);

}

void ChangeBoidsSpeed(int type)
{
	for(int i = 0; i < boids.size(); i++)
	{
		if(type == 0)
		{
			boids[i].setMaxSpeed(boids[i].getMaxSpeed() - 0.1);
		}
		else
		{
			boids[i].setMaxSpeed(boids[i].getMaxSpeed() + 0.1);
		}


		//cout << boids[i].getMaxSpeed() <<endl;
	}
}

void changeForce(int param, int change)
{
	for(int i = 0; i < boids.size(); i++)
	{
		if (param == 0 && change == 0)
		{
			boids[i].setMaxForce(0,boids[i].getMaxForce(0) - 0.01);
		}
		else if (param == 0 && change == 1)
		{
			boids[i].setMaxForce(0,boids[i].getMaxForce(0) + 0.01);
		}
		else if (param == 1 && change == 0)
		{
			boids[i].setMaxForce(1,boids[i].getMaxForce(1) - 0.01);
		}
		else if (param == 1 && change == 1)
		{
			boids[i].setMaxForce(1,boids[i].getMaxForce(1) + 0.01);
		}
		else if (param == 2 && change == 0)
		{
			boids[i].setMaxForce(2,boids[i].getMaxForce(2) - 0.01);
		}
		else if (param == 2 && change == 1)
		{
			boids[i].setMaxForce(2,boids[i].getMaxForce(2) + 0.01);
		}
	}
}

void changePerc(int param, int change)
{
	for(int i = 0; i < boids.size(); i++)
	{
		if (param == 0 && change == 0)
		{
			boids[i].setMaxPerc(0,boids[i].getMaxPerc(0) - 0.1);
		}
		else if (param == 0 && change == 1)
		{
			boids[i].setMaxPerc(0,boids[i].getMaxPerc(0) + 0.1);
		}
		else if (param == 1 && change == 0)
		{
			boids[i].setMaxPerc(1,boids[i].getMaxPerc(1) - 0.1);
		}
		else if (param == 1 && change == 1)
		{
			boids[i].setMaxPerc(1,boids[i].getMaxPerc(1) + 0.1);
		}
		else if (param == 2 && change == 0)
		{
			boids[i].setMaxPerc(2,boids[i].getMaxPerc(2) - 0.1);
		}
		else if (param == 2 && change == 1)
		{
			boids[i].setMaxPerc(2,boids[i].getMaxPerc(2) + 0.1);
		}
	}	
}

void ClacRayComp(int x, int y)
{
	int viewport[4];
	double modelview[16];
	double projection[16];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	double yAdjusted = viewport[3] - y;

	double objx, objy, objz;

	gluUnProject(x,yAdjusted,0,modelview,projection,viewport, &objx, &objy, &objz);
	rayNear[0] = objx;
	rayNear[1] = objy;
	rayNear[2] = objz;

	gluUnProject(x,yAdjusted,1,modelview,projection,viewport, &objx, &objy, &objz);
	rayFar[0] = objx;
	rayFar[1] = objy;
	rayFar[2] = objz;
}

bool rayCol(float d0, float d1, float d2, float ori0, float ori1, float ori2)
{
	for(int i = 0; i < objects.size(); i++)
	{
		bool coli = objects[i].sphereCol(d0,d1,d2,ori0,ori1,ori2);

		if(coli == true)
		{
			selectedObj = i;
			return true;
		}
	}

	selectedObj = -1;
	return false;
}


bool rayColBoid(float d0, float d1, float d2, float ori0, float ori1, float ori2)
{
	for(int i = 0; i < boidSph.size(); i++)
	{
		bool coli = boidSph[i].sphereCol(d0,d1,d2,ori0,ori1,ori2);

		if(coli == true)
		{
			selectedBoid = i;
			return true;
		}
	}

	selectedBoid = -1;
	return false;
}

void changeAv(float amount)
{
	for(int i =0; i < boids.size(); i++)
	{
		boids[i].maxForceAv += amount;
	}
}

void mouseClick(int button, int state, int x, int y)
{
	if(glutGetWindow() == wind2 && state == 0)
	{
		int button = GUI.buttonClick(x,500-y);
		if( button > 0)
		{
			switch (button){
				case 1:
					ChangeBoidsSpeed(0);
					break;
				case 2:
					ChangeBoidsSpeed(1);
					break;
				case 3:
					changeForce(0,0);
					break;
				case 4:
					changeForce(0,1);
					break;
				case 5:
					changeForce(1,0);
					break;
				case 6:
					changeForce(1,1);
					break;
				case 7:
					changeForce(2,0);
					break;
				case 8:
					changeForce(2,1);
					break;
				case 9:
					changePerc(0,0);
					break;
				case 10:
					changePerc(0,1);
					break;
				case 11:
					changePerc(1,0);
					break;
				case 12:
					changePerc(1,1);
					break;
				case 13:
					changePerc(2,0);
					break;
				case 14:
					changePerc(2,1);
					break;
				case 15:
					changeAv(-1);
					break;
				case 16:
					changeAv(1);
			}


		}
	}
    
	else if(glutGetWindow() == wind1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseDown = 1;
		clickXPos = x;
		clickYPos = y;
		cout << "click down";
	}
	
	else if(glutGetWindow() == wind1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		mouseDown = 0;
		cout << "click up " << mouseDown << " ";
	}

	if(state == GLUT_UP)
	{
		ClacRayComp(x,y);

		float dir[3] = {rayFar[0] - rayNear[0],rayFar[1] - rayNear[1],rayFar[2] - rayNear[2]};
		bool selected = rayCol(dir[0], dir[1], dir[2], rayNear[0], rayNear[1], rayNear[2]);

		if(selected == true && button == GLUT_RIGHT_BUTTON)
		{
			objects.erase(objects.begin()+selectedObj);
			selectedObj = -1;
		}

		if(pause == true)
		{
			bool BoidSelected = rayColBoid(dir[0], dir[1], dir[2], rayNear[0], rayNear[1], rayNear[2]);

			if(BoidSelected == true && button == GLUT_RIGHT_BUTTON)
			{
				boidSph.erase(boidSph.begin()+selectedBoid);
				boids.erase(boids.begin()+selectedBoid);
				selectedBoid = -1;
			}
		}
	}

	glutSetWindow(wind2);
	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
	glutSetWindow(wind1);
}

Vec3D projectToSphere(float x, float y){
	float z = sqrt(1 - x*x - y*y);
	return Vec3D(x,y,z);
}

void mouseMotion(int x, int y){
	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	if(mouseDown && x > 0 && y > 0 && x < width && y < height){
		// cout << mouseDown << "\n";
		float centerX = width/2;
		float centerY = height/2;
		float realY = height-y;
		float r = 620;

		float adjustedClickXPos = clickXPos - centerX;
		float adjustedClickYPos = -(clickYPos - centerY);
		float adjustedX = x - centerX;
		float adjustedY = - (y-centerY);

		float normalizedClickXPos = adjustedClickXPos/r;
		float normalizedClickYPos = adjustedClickYPos/r;
		float normalizedX = adjustedX/r;
		float normalizedY = adjustedY/r;

		float magnitude1 = sqrt(normalizedX*normalizedX + normalizedY*normalizedY);
		float magnitude2 = sqrt(normalizedClickXPos*normalizedClickXPos + normalizedClickYPos*normalizedClickYPos);

		Vec3D p1,p2;

		if(magnitude1 >= 1){
			normalizedX = normalizedX/magnitude1;
			normalizedY = normalizedY/magnitude1;
			p2 = Vec3D(normalizedX, normalizedY, 0.001);
		}
		else{
			p2 = projectToSphere(normalizedX,normalizedY);
		}

		if(magnitude2 >= 1){
			normalizedClickXPos = normalizedClickXPos/magnitude2;
			normalizedClickYPos = normalizedClickYPos/magnitude2;
			p1 = Vec3D(normalizedClickXPos, normalizedClickYPos, 0.001);
		}
		else{
			p1 = projectToSphere(normalizedClickXPos, normalizedClickYPos);
		}

		// cout << normalizedClickXPos << " " << normalizedClickYPos << "\n";
		// cout << p1.z << " \n";

		Quaternion q = Quaternion::vectorsToQuaternion(p1,p2);
		if(rotationQuaternion.a == -1){
			rotationQuaternion = q;
		}
		else{
			rotationQuaternion = rotationQuaternion.multiply(q);
		}
		rotationMatChanged = 1;

		clickXPos = x;
		clickYPos = y;
	}
}


void renderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x,y,z);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void guiDisp(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0,500,0,500);

	for(int i = 0; i < numOfButtons; i++)
	{
		GUI.addButton(buttonCord[i][0],buttonCord[i][1],10,i+1,i % 2);
	}

	for(int i = 0; i < GUI.buttons.size(); i += 2)
	{
		if(GUI.buttonClick(GUI.buttons[i],GUI.buttons[i+1])%2 == 0)
		{
			glColor3f(0,1,0);
		}
		else
		{
			glColor3f(1,0,0);
		}
		glBegin(GL_POINTS);
			glVertex2f(GUI.buttons[i],GUI.buttons[i+1]);
		glEnd();
	}

	glColor3f(1,1,1);

	char c[100];
	sprintf(c, "Max Speed: %.2f",boids[0].getMaxSpeed());
	renderBitmapString(170,450,0,GLUT_BITMAP_TIMES_ROMAN_24, c);

	sprintf(c, "Max Force Align: %.2f",boids[0].getMaxForce(0));
	renderBitmapString(25,340,0,GLUT_BITMAP_TIMES_ROMAN_24, c);

	sprintf(c, "Max Force Coh: %.2f",boids[0].getMaxForce(1));
	renderBitmapString(25,240,0,GLUT_BITMAP_TIMES_ROMAN_24, c);

	sprintf(c, "Max Force Sep: %.2f",boids[0].getMaxForce(2));
	renderBitmapString(25,140,0,GLUT_BITMAP_TIMES_ROMAN_24, c);

	sprintf(c, "Max Perc Align: %.2f",boids[0].getMaxPerc(0));
	renderBitmapString(270,340,0,GLUT_BITMAP_TIMES_ROMAN_24, c);

	sprintf(c, "Max Perc Coh: %.2f",boids[0].getMaxPerc(1));
	renderBitmapString(270,240,0,GLUT_BITMAP_TIMES_ROMAN_24, c);

	sprintf(c, "Max Perc Sep: %.2f",boids[0].getMaxPerc(2));
	renderBitmapString(270,140,0,GLUT_BITMAP_TIMES_ROMAN_24, c);

	sprintf(c, "Max Force Avoid: %.2f",boids[0].maxForceAv);
	renderBitmapString(150,65,0,GLUT_BITMAP_TIMES_ROMAN_24, c);



	glutSwapBuffers();
}

void minDisp(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(-500,500,-500,500);

	float totalSpeed;
	glPointSize(5);
	for(int i = 0; i < boids.size(); i++)
	{
		float green;
		float red;
		totalSpeed += boids[i].vel.getMag();

		if(boids[i].pos.getY() <= 0){green = -boids[i].pos.getY()/50; red = 0;}
		else{red = boids[i].pos.getY()/50; green = 0;}

		glBegin(GL_POINTS);
			glColor3f(red,green,1);
			glVertex2f(boids[i].pos.getX()*10,boids[i].pos.getZ()*10);
		glEnd();
	}

	for(int i = 0; i < objects.size(); i++)
	{
		glPushMatrix();
			glColor3f(1,1,1);
			glTranslatef(objects[i].pos[0]*10,objects[i].pos[2]*10,0);
			glScalef(10,10,0);
			glutSolidSphere(objects[i].radius-3,100,100);
		glPopMatrix();
	}

	glColor3f(1,1,1);
	char c[100];
	sprintf(c, "Average Speed: %.2f",totalSpeed/boids.size());
	renderBitmapString(50,450,0,GLUT_BITMAP_TIMES_ROMAN_24, c);

	glutPostRedisplay();
	glutSwapBuffers();
}

void createWind()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
    glutInitWindowPosition(0,800);
    wind2 = glutCreateWindow("GUI");

    glutDisplayFunc(guiDisp);

}

void createWind2()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
    glutInitWindowPosition(2000,800);
    wind3 = glutCreateWindow("Minimap");

    glutDisplayFunc(minDisp);

}

void addObject()
{
	Object sphere(0,0,0,10);
	objects.push_back(sphere);
	selectedObj = objects.size() -1;
}

void keyboardFunc(unsigned char key, int x, int y){
    if(glutGetModifiers() ==  GLUT_ACTIVE_SHIFT && key == 'R'){
		
    }

    if(glutGetModifiers() ==  GLUT_ACTIVE_SHIFT && key == 'Q'){
        exit(0);
    }

	switch (key){
		case 'r':
			
			break;
		case 'a':{
			Boid temp;
			Object sphere(temp.pos.getX(),temp.pos.getY(),temp.pos.getZ(),2);
		
			boidSph.push_back(sphere);
			boids.push_back(temp);
			break;}
		case '-':
			objects[selectedObj].radius -= 1;
			drawObjects();
			break;
		case '=':
			objects[selectedObj].radius += 1;
			drawObjects();
			break;
		case ' ':
			trackingBoid = !trackingBoid;
			break;
		case 'p':
			if (pause == true){pause = false;}
			else { pause = true;}
			break;
		default:
			break;
	}

	if(key == '1')
	{
		addObject();
	}
}

void arrowKeys(int key, int x, int y){

	int mod = glutGetModifiers();

	if (trackingBoid){
		if (key == GLUT_KEY_RIGHT){
			trackingIndex = (trackingIndex < boids.size()-1) ? trackingIndex + 1: 0;
		}
		else if (key == GLUT_KEY_LEFT){
			trackingIndex = (trackingIndex > 0) ? trackingIndex - 1: boids.size()-1;
		}
	}
	else{
		if (key == GLUT_KEY_RIGHT){
			trackingIndex = (trackingIndex < boids.size()-1) ? trackingIndex + 1: 0;
		}
		else if (key == GLUT_KEY_LEFT){
			trackingIndex = (trackingIndex > 0) ? trackingIndex - 1: boids.size()-1;
		}
	}

	if (key == GLUT_KEY_RIGHT && selectedObj == -1){
		yRotation -= pi/30;
	}

    else if (key == GLUT_KEY_LEFT && selectedObj == -1){
		yRotation += pi/30;
	}

    else if (key == GLUT_KEY_UP && selectedObj == -1){
		if(xRotation > pi/20){
			xRotation -= pi/30;
		}
	}

    else if (key == GLUT_KEY_DOWN && selectedObj == -1){
		if(xRotation < 19*pi/20){
			xRotation += pi/30;
		}
	}



	else if (mod == GLUT_ACTIVE_SHIFT && key == GLUT_KEY_UP && selectedObj != -1){
		objects[selectedObj].pos[2] -= 1;
		drawObjects();
	}
	else if (mod == GLUT_ACTIVE_SHIFT && key == GLUT_KEY_DOWN && selectedObj != -1){
		objects[selectedObj].pos[2] += 1;
		drawObjects();
	}
	else if (key == GLUT_KEY_RIGHT && selectedObj != -1){
		objects[selectedObj].pos[0] += 1;
		drawObjects();
	}

    else if (key == GLUT_KEY_LEFT && selectedObj != -1){
		objects[selectedObj].pos[0] -= 1;
		drawObjects();
	}

    else if (key == GLUT_KEY_UP && selectedObj != -1){
		objects[selectedObj].pos[1] += 1;
		drawObjects();
	}

    else if (key == GLUT_KEY_DOWN && selectedObj != -1){
		objects[selectedObj].pos[1] -= 1;
		drawObjects();
	}

}

void applyTexture(char * filename, int index){
	if (filename == "obj/goose.bmp"){
		objFile = objValues.LoadFile("obj/goose.obj");
		mesh0 = objValues.LoadedMeshes[0];
		mesh = objValues.LoadedMeshes[1];
		cout << objValues.LoadedMeshes.size() << flush; 
		unsigned char * file = stbi_load("obj/goose.bmp", &mWidth[index], &mHeight[index], &n[index], 0);
		tex[index] = file;
	}
	else if (filename == "obj/sky.bmp"){ 
		unsigned char * file = stbi_load("obj/sky.bmp", &mWidth[index], &mHeight[index], &n[index], 0);
		tex[index] = file;		
	}
	else { 
		GLubyte * file = LoadPPM(filename, &mWidth[index], &mHeight[index]);
		tex[index] = file;
	}
	glBindTexture(GL_TEXTURE_2D, textureList[index]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);		
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth[index], mHeight[index], 0, GL_RGB, GL_UNSIGNED_BYTE, tex[index]);
}

void getTextures() { 
	glGenTextures(4, textureList);
	applyTexture("obj/goose.bmp", 0);
	applyTexture("obj/grass2.ppm", 1);
	applyTexture("obj/sky.bmp", 2);
	applyTexture("obj/wood2.ppm", 3);
}

int main(int argc, char** argv) {
	//putenv((char  *) "__GL_SYNC_TO_VBLANK=1" );
    glutInit(&argc, argv);
    for(int i = 0; i < 50;i++)
	{
		Boid temp;
		Object sphere(temp.pos.getX(),temp.pos.getY(),temp.pos.getZ(),2);

		boidSph.push_back(sphere);
		boids.push_back(temp);
	}
    createWind();
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboardFunc);

    createWind2();
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboardFunc);
    
    glutInitWindowSize(1000,1000);
    glutInitWindowPosition(800,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    wind1 = glutCreateWindow("Boids");
	
	glEnable(GL_TEXTURE_2D);
	//gluQuadricTexture(quadric, GL_TRUE);
	getTextures();
    srand(time(NULL));

	Vec3D v1(2,3,4);
	Quaternion test(1,v1);

	Vec3D v2(3,2,1);
	Quaternion test2(4,v2);

	Quaternion t3 = test.multiply(test2);

    glutReshapeFunc(handleReshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);

	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(arrowKeys);
    glutTimerFunc(17,FPS,0);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);	
	glEnable(GL_LIGHT0);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glutMainLoop();
    return 0;
}
