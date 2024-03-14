For all parts (and subparts for separate functionality), navigate to the folder with the desired part in the terminal then follow these steps:

type "make" in the terminal to run the makefile

then type "./main" to run the program

close the program by closing the window

Part 1: the boid will match the mouse velocity only when the mouse is within the SFML window

Part 2: the boid will position and orienation match towards the point in which you click on the SFML window

Part 3: the boid will wander around the screen with no input

Part 4: the boids will flock with no input



Please note that the screen wrapping that I have implemented does not actually change the Kinematic data located in the objects, just the position that SFML uses to draw them, this is obvious in Part 4 when some boids that wrap can come into contact with boids who have not wrapped and will not interact with each other.