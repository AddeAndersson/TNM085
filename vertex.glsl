#version 330 core
uniform float time;
uniform mat4 P; //Perspective
uniform mat4 MV; //Model view (Camera)

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in vec2 StartPos;
layout(location = 4) in mat4 InstanceMatrix;

out vec3 N;
out vec2 st;

void main() {
    vec3 transformedNormal = mat3(MV)*mat3(InstanceMatrix)*Normal;
    N = normalize(transformedNormal);

if(gl_InstanceID == 2){
	gl_Position = P*MV*InstanceMatrix*vec4(Position + vec3(time, 0.0, 0.0), 1.0); //T is matrix
}
else{
	gl_Position = P*MV*InstanceMatrix*vec4(Position + vec3(StartPos, 0.0), 1.0); //T is matrix
}

    
    st = TexCoord;
}
