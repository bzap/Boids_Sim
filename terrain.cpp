#include "terrain.h"
#include "vec3D.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <cstdlib>

Terrain::Terrain(int inp_gridSize, int inp_startingHeight){
    gridSize = inp_gridSize;
    startingHeight = inp_startingHeight;

    srand((int) time(0));
    allocateGridVerts();
    addTerrain(30);
    // addBuildings(1, 100, 40);
    computeNormals();
}

void Terrain::allocateGridVerts(){
    int n = gridSize/2;
    for(int i = -n; i < n; i ++){
        vector <Vec3D> row;
        for(int j = -n; j < n; j++){
            Vec3D vertex(i, startingHeight, j);
            row.push_back(vertex);
        }
        grid.push_back(row);
    }
}

void Terrain::addTerrain(int nIterations){
    for(int n = 0; n < nIterations; n++){
        int xPos = rand() % gridSize - gridSize/2;
        int zPos = rand() % gridSize - gridSize/2;
        float r = min(gridSize/5, 30);
        float maxDisp = 15;



        for (int i = 0; i < gridSize; i ++){
            for(int j = 0; j < gridSize; j ++){

                Vec3D cPoint = grid[i][j];

                float dist = sqrt(pow(xPos - cPoint.getX(), 2) + pow(zPos - cPoint.getZ(), 2));
                float pd = dist * 2 / r;

                if (fabs(pd) <= 1){
                    float delta = maxDisp/2 + cos(pd*pi)*maxDisp/2;
                    grid[i][j].y += delta;
                    minHeight = (grid[i][j].getY() < minHeight) ? grid[i][j].getY() : minHeight;
                    maxHeight = (grid[i][j].getY() > maxHeight) ? grid[i][j].getY() : maxHeight;
                }
            }

        }
    }
}

void Terrain::addBuildings(int nBuildings, int maxHeight, int maxWidth){
    for(int i = 0; i < nBuildings; i ++){
        int h = rand() % 3*maxHeight/4 + maxHeight/4;
        int w = rand() % 3*maxWidth/4 + maxWidth/4;

        int xPos = rand() % (gridSize-w-1);
        int yPos = rand() % (gridSize-w-1);

        Building newBuilding;
        newBuilding.height = h;
        newBuilding.width = w;
        newBuilding.cornerX = xPos;
        newBuilding.cornerY = yPos;
        buildings.push_back(newBuilding);

        // Add buildings to grid
        for(int x = 0; x < w; x++){
            for(int y = 0; y < w; y++){
                grid[xPos+x][yPos+y].y = startingHeight + h;
            }
        }
    }
}

void Terrain::computeNormals(){
    for (int i = 0; i < grid.size()-2; i ++){
        vector <Vec3D> row;
        for(int j = 0; j < grid.size()-2; j ++){
            Vec3D vecA = Vec3D::createVector(grid[i][j], grid[i+1][j]);
            Vec3D vecB = Vec3D::createVector(grid[i][j], grid[i][j+1]);
            Vec3D firstNormal = vecB.crossProduct(vecA); 

            Vec3D vecC = Vec3D::createVector(grid[i+1][j+1], grid[i+1][j]);
            Vec3D vecD = Vec3D::createVector(grid[i+1][j+1], grid[i][j+1]);
            Vec3D secondNormal = vecC.crossProduct(vecD); 

            Vec3D avgNormal((firstNormal.getX() + secondNormal.getX())/2, (firstNormal.getY() + secondNormal.getY())/2, (firstNormal.getZ() + secondNormal.getZ())/2);
            row.push_back(avgNormal);
        }
        normals.push_back(row);
    }
}