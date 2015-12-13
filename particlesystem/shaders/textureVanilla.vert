#version 400

layout(location = 0) in vec3 position;
out vec3 uv;
uniform mat4 model, view, projection;
//billboard size uniform
//pass in look vector
//rotate around the look vector, multiply by original position
void main() {
    uv = position;
    //for later, I'm assuming that billboarding will change the line after to the folowing:
    //gl_Position = gl_Position + CameraRight_worldspace * squareVertices.x * m_scale + CameraUp_WorldSpace*squareVertices.y *m_scale?
    //mat4 modelview = view * model;
    gl_Position = model*projection * view* vec4(position, 1.0);

}
