#version 400

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;
out vec2 uv;
uniform mat4 model, view, projection;
//billboard size uniform
//pass in look vector
//rotate around the look vector, multiply by original position
void main() {
    uv = texCoord;
    //for later, I'm assuming that billboarding will change the line after to the folowing:
    //gl_Position = gl_Position + CameraRight_worldspace * squareVertices.x * m_scale + CameraUp_WorldSpace*squareVertices.y *m_scale?
    mat4 modelview = view * model;
    modelview[0][0]=1;
    modelview[0][1]=0;
    modelview[0][2]=0;
    modelview[1][0]=0;
    modelview[1][1]=1;
    modelview[1][2]=0;
    modelview[2][0]=0;
    modelview[2][1]=0;
    modelview[2][2]=1;
    gl_Position = projection * modelview* vec4(position, 1.0);

}
