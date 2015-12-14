# Graphics-Final-Project
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
billboarding
The implementation of the particle system was based on the particle lab. It likely uses the same billboarding technique as well.
 
It generally  consists of particle managers, which update and reset particles as their life decays. The properties of the particles, such as the speed of the particle emissions or the initial position of the emitter can be adjusted on the creation of a particle manager. 

A list of these managers is in view, which handles openGL operations for this particle system as well. 

For future reference, the force parameter generally focuses the particles on the direction of the force.


Terrain Generation:
The terrain is procedurally generated using the diamond-square algorithm on a 1025x1025 grid of points. This generates a height for each position that is fairly continuous, though the balance between smoothness and interesting terrain features can be tweaked by playing with initialNoise and noiseFactor in terrain.cpp. I then approximated a normal for each point using the same technique from lab 5 in which the normals of the 8 adjacent triangles were averaged, and used this for diffuse lighting.

The diffuse lighting was then blended with a texture map to create the final color. However, because uv-coordinate mapping caused very obvious stretching when two adjacent points had very different height values (which often happened around the central point or other points determined before noise was dampened), I used tri-planar texture mapping. This required applying the texture three separate times (once in each of the three major positive axes, blended so that the total contributions summed to one using the normal of the groun).

This led to interesting terrain, but it was difficult to see all the fine details without distracting from the scene. To solve this, I added the option of turning off the terrain texture and rendering using only the diffuse + ambient lighting model, which shows how detailed the terrain is.


Skybox:
Extra feature implemented in spare time. It creates a cubemap and a cube through points, then essentially uses glTexImage2D like a texture map for each face. It also has its own wrapping and filtering thorugh glTexParameteri. 

Images:

Some skybox/cubemap pictures and implementation detials came from this tutorial: http://learnopengl.com/#!Advanced-OpenGL/Cubemaps and http://www.custommapmakers.org/skyboxes.php. We used the plaza cubemap from the 123 images folder in the end. It also uses its own shaders, which sample the cubemap images.

Particle images came from the particle lab particle images and one from here https://tutorialsplay.com/opengl/wp-content/uploads/sites/2/2014/04/starfield-300x300.jpg.

Green particle:http://opengameart.org/node/7709
Fireball particle:http://opengameart.org/node/7724

Terrain texture came from http://kay-vriend.blogspot.com/2012/09/easy-automated-normal-and-specular-map.html

Bugs:

Particle system color settings may not have an effect. They primarily use textures. 
