#version 330 core
//vec3 L is the light direction
//vec3 V is the view direction - (0 ,0 ,1) in view space
//vec3 N is the normal
//vec3 R is the computed reflection direction
//float n is the " shininess " parameter
//vec3 ka is the ambient reflection color
//vec3 Ia is the ambient illumination color
//vec3 kd is the diffuse surface reflection color
//vec3 Id is the diffuse illumination color
//vec3 ks is the specular surface reflection color
//vec3 Is is the specular illumination color

uniform float time;

in vec3 N;

out vec4 finalcolor;

void main() {

    vec3 ka = vec3(0.0, 1.0, 1.0);
    vec3 Ia = vec3(1.0, 0.5, 0);
    vec3 kd = vec3(1.0, 1.0, 0);
    vec3 Id = vec3(1.0, 0, 0);
    vec3 ks = vec3(1.0, 1.0, 1.0);
    vec3 Is = vec3(1.0, 1.0, 1.0);
    
    float n = 5;

    vec3 V = normalize(vec3(0.0, 0.0, 1.0));
    vec3 L = normalize(vec3(1.0, 1.0, 1.0));

    vec3 R = 2.0*dot(N,L)*N - L;
    float dotNL = max(dot(N,L), 0.0);
    float dotRV = max(dot(R,V), 0.0);
    if(dotNL == 0.0){
        dotRV = 0.0;
    }
    vec3 shadedColor = Ia*ka + Id*kd*dotNL + Is*ks*pow(dotRV, n);
    finalcolor = vec4(shadedColor, 1.0);
}
