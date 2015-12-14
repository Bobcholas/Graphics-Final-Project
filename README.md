# Graphics-Final-Project
User control:

currently, the user is capable of dragging the camera around using the mouse and zooming out with the wheel. 
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

Skybox:
Extra feature implemented in spare time. It creates a cubemap and a cube through points, then essentially uses glTexImage2D like a texture map for each face. It also has its own wrapping and filtering thorugh glTexParameteri. 

Images:

Some skybox/cubemap pictures and implementation detials came from this tutorial: http://learnopengl.com/#!Advanced-OpenGL/Cubemaps and http://www.custommapmakers.org/skyboxes.php. We used the plaza cubemap from the 123 images folder in the end. It also uses its own shaders, which sample the cubemap images.

Particle images came from the particle lab particle images and one from here https://tutorialsplay.com/opengl/wp-content/uploads/sites/2/2014/04/starfield-300x300.jpg.

Green particle:http://opengameart.org/node/7709
Fireball particle:http://opengameart.org/node/7724

Bugs:

Particle system color settings may not have an effect. They primarily use textures. 
