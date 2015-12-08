#version 400

in vec3 position;
// TODO (Task 4): Add a new in variable for the color attribute
in vec3 color;
// TODO (Task 5): Declare a new out vec3 to send the color attribute
out vec3 tosend;
void main() {
    gl_Position = vec4(position, 1.0);
    // TODO (Task 5): Set an output variable to a per-vertex color attribute.
    //color = vec3(0.7f,0.7f,0.7f);
    tosend = color;// ?
}
