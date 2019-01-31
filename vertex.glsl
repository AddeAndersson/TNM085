#version 330 core
uniform float time;
uniform mat4 T;
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
out vec3 interpolatedColor;

void main() {
    //Apply matrix transformation with matrix T
    interpolatedColor = Color;
    gl_Position = T*vec4(Position, 1.0);
}
