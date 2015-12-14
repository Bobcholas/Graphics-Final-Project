#version 400 core
in vec2 uv;
in vec3 WS_position; // world-space position
in vec3 WS_normal;   // world-space normal

uniform sampler2D img;

out vec4 fragColor;

void main() {
    vec3 WS_toLight = normalize(vec3(10.0) - WS_position);
    vec3 diff = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));

    fragColor = vec4(diff, 0);
}
