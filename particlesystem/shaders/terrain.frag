#version 400 core
in vec2 uv;
in vec3 WS_position; // world-space position
in vec3 WS_normal;   // world-space normal

uniform sampler2D img;

out vec4 fragColor;

void main() {
    vec3 WS_toLight = normalize(vec3(10.0) - WS_position);
    vec3 diff = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));

    vec3 blending = abs( WS_normal );
    blending = normalize(max(blending, 0.00001)); // Force weights to sum to 1.0
    float b = (blending.x + blending.y + blending.z);
    blending /= vec3(b, b, b);

    vec4 xaxis = texture( rockTexture, coords.yz);
    vec4 yaxis = texture( rockTexture, coords.xz);
    vec4 zaxis = texture( rockTexture, coords.xy);

    vec4 tex = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;

    vec4 diff4 = vec4(diff, 0);
    fragColor = diff*.2 + .7*tex;
}
