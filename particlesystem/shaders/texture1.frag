#version 400 core

in vec3 uv;

uniform samplerCube tex;

out vec4 fragColor;

void main(){
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);

	 // TODO: [Task 8] Sample the texture "tex" at the given UV-coordinates.
    fragColor = texture(tex,uv);
}
