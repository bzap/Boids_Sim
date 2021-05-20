#ifndef TERRAIN_H
#define TERRAIN_H

#include "vec3D.h"
#include <iostream>
#include <vector>
using namespace std;

class Terrain {
public:

    struct Building{
        int height;
        int width;
        int cornerX;
        int cornerY;
    };

    int gridSize;
    int startingHeight;
    float minHeight, maxHeight;
    float pi = 3.14159;
    std::vector < vector <Vec3D> > grid;
    std::vector < vector <Vec3D> > normals;
    std::vector <Building> buildings;

	Terrain(int gridSize, int startingHeight);

	void allocateGridVerts();
    void drawGrid();
    void addTerrain(int nIterations);
    void addBuildings(int nBuildings, int maxHeight, int maxWidth);
    void computeNormals();

};


#endif