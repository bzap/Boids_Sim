boid: main.cpp FORCE
	g++ main.cpp boid.cpp vec3D.cpp gui.cpp object.cpp quaternion.cpp matrix.cpp terrain.cpp PPM.cc -o main -lGL -lGLU -lglut -std=c++11
	./main

.PHONY: FORCE
FORCE: 

clean: 
	-rm -rf main 
