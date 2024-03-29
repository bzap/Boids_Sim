Animal flocking model (Boid Simulation)

![logo]

[logo]: https://github.com/bzap/3GC3---Boids-/blob/master/sample.png "Demo of simulation"



For our final project we have decided to create a program that graphically simulates the flocking behaviour of bird like animals in an open environment. This flocking behaviour is achieved by programming each Boid, an individual bird/animal, to react to its environment based on its position and velocity relative to its neighbouring Boids. This can be done by each Boid abiding by the following 3 simple steering behaviours:

1. Separation: Steer to avoid crowding local flock mates
2. Alignment: Steer towards the average heading of local flock mates
3. Cohesion: Steer towards the average position of local flock mates

Each Boid is also given a viewing neighbourhood, which restricts the Boid to make decisions based solely on other Boids in the viewing radius. To put it simply, Boids are unaware of other Boids outside of their viewing neighbourhood. The viewing distance is defined by a distance starting from the center of the Boid. Other factors like object avoidance can also be added to the simulation.

OUR IMPLEMENTATION:

User Functionality:
- Quaternion camera control: You can view the environment from different angles by holding down the left mouse button and dragging the mouse across the screen
- Objects: An object can be added by pressing the '1' key. To select the Object, u must left click on it, Once an object is selected u can press the - or = button to decrease or increase the objects radius. You can use the arrow keys to move the object on the x and y axis. To move the object on the z axis, hold shift and press the up and down arrow keys. Right clicking the object will delete it. Boids will avoid placed objects
- Pausing simulation: Press 'p' to pause the simulation. You can press 'p' again to un-pause
- Adding/deleting Boids: To delete or add a Boid, u must first pause the simulation. Once it is paused, pressing the 'a' key will add a Boids and right clicking a Boid will delete it.
- GUI Window: The GUI can be used to change the simulation parameter values. Each label indicates the current parameter value. The red and green button underneath the label will decrease and increase the parameters value respectively.
- Mini-map: The mini-map shows the 2D version of the simulation (as if you were looking at the simulation from below). The mini-map also includes the current average speed of the Boids.
- Boids POV: pressing space key will give shift the camera to the point of view of a Boid. From this POV, you can cycle between the POV of different bonds using the arrow keys. To go back to normal view, press space again.

Boid Functionality:
- Steering behaviours: The Boids follow the three rules explained above 
- Object avoidance: The Boids will avoid objects that are in their flight path
- Terrain avoidance: The Boids will avoid the terrain in their flight path (if a new terrain is generated it will adapt to the new terrain)

Features from list:
- Lighting [5 %]
- Textures [10 %]
- Picking[10 %]
- Quaternion camera control [10%]
- Terrain generation [10%]
- Non geometric primitives (bitmaps for GUI) [10 %]
- Our Own ideas:
	- Mini-map
	- Boid camera POV
	- Double buffering (Improve performance)

To compile:
- used regular make file.

Group members:
-Ravi Patel
-Jan Ollers
-Linas Aleknevicius




