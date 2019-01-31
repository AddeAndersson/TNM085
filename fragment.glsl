#version 330 core
uniform float time;
out vec4 finalcolor;
in vec3 interpolatedColor;

void main() {
    finalcolor = vec4(interpolatedColor, 1.0);
}
