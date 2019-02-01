#version 330 core
uniform float time;
uniform mat4 T;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;

out vec3 N;
out vec2 st;

void main() {
    vec3 transformedNormal = mat3(T) * Normal;
    N = normalize(transformedNormal);
    gl_Position = T*vec4(Position, 1.0); //T is matrix
    st = TexCoord;
}
