# Graphics-Final-Project
README
rncunnin, zloery, sch3
User control:

currently, the user is capable of dragging the camera around using the mouse and zooming out with the wheel. 
1 and 2 switch textures for terrain on and off
up/down keys increase/decrease particle simulation speed
left/right keys decrease/increase particle randomness

Particle System:
This particle system has several features:
interchangeable texture mapping
rudimentary physics
blending
multiple particle emitters
billboarding (based on this tutorial which was sent by the mentor TA: http://www.geeks3d.com/20140807/billboarding-vertex-shader-glsl/ )
The implementation of the particle system was based on the particle lab. It likely uses the same billboarding technique as well.
 
It generally  consists of particle managers, which update and reset particles as their life decays. The properties of the particles, such as the speed of the particle emissions or the initial position of the emitter can be adjusted on the creation of a particle manager. 

A list of these managers is in view, which handles openGL operations for this particle system as well. 

For future reference, the force parameter generally focuses the particles on the direction of the force.

Skybox:
Extra feature implemented in spare time. It creates a cubemap and a cube through points, then essentially uses glTexImage2D like a texture map for each face. It also has its own wrapping and filtering thorugh glTexParameteri. 

Images:

Some skybox/cubemap pictures and implementation detials came from this tutorial: http://learnopengl.com/#!Advanced-OpenGL/Cubemaps and http://www.custommapmakers.org/skyboxes.php. We used the plaza cubemap from the 123 images folder in the end. It also uses its own shaders, which sample the cubemap images.

Particle images came from the particle lab particle images and one from here https://tutorialsplay.com/opengl/wp-content/uploads/sites/2/2014/04/starfield-300x300.jpg.

Green particle:http://opengameart.org/node/7709
Fireball particle:http://opengameart.org/node/7724


Procedural dudes: 

The people scattered through the landscape are wizards, because that's cooler than statues. 
They are complex figures composed of multiple objects.
Each one maintains a list of the objects it comprises and how to draw them (as in sceneview). 

They are created and posed randomly; after much trial and error with parameters, their arms now are usually in cool spellcasting stances.
They can be placed in any location, and the constructor takes as an argument a transformation matrix that should be applied to all elements; this matrix is used in our project to place them on the surface of the terrain, standing at an angle equal to the terrain's normal at that point. 

Some things that were tried (but discarded because they looked weird) were differently shaped body parts; for example, cube/sphere limbs can be easily reenabled, but the resulting limbs look much more robotic than wizardly. So, we stuck with cylinders and spherical joints for the limbs, so that everything would look nice and smooth.

The random creation process generates outwards from the torso; the body creates a head and 4 limbs, and guarantees that the arms and legs will be similar lengths by requiring them to have the same number of joints. 
The limb-building process counts down joints until the limb is done, then puts a particle emitter on the end with some wobble from the previous direction so it's not just shooting straight out of the ends of the arms.
The process itself attempts to keep limbs roughly within a plane; this mimics real limbs and makes the arms look more natural and less like horrible abominations.


Bugs:

Particle system color settings may not have an effect. They primarily use textures. 
Particle updates and resetting calls on the at method a lot. I tried to abstract this out, but this made particles vanish after a period of time. 
Particle active parameter is unused.
Putting the texture map QImage in the initializer list of particle manager resulted in crashes on some systems. This has been commented out.
