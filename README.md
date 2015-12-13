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
