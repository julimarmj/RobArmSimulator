#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 3) in vec3 inColors;

out vec4 V_Colors;
out vec3 V_Normal;
out vec3 V_FragPos;  

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

out VS_OUT {
    vec3 N;
    vec3 L;
    vec3 V;
} vs_out;

uniform vec3 lightDirection = vec3(0.0, 5.0, 0.0);

uniform mat4 u_DHM;
uniform mat4 u_DHM_I;
uniform float u_AngC;
float AngC = -radians(u_AngC);

void main()
{
    mat4 Rx = mat4( 1,       0,       0, 0,
				    0, cos(AngC), -sin(AngC), 0,
				    0, sin(AngC),  cos(AngC), 0,
				    0,           0,            0, 1 );

    mat4 Maux = mat4(1);
    Maux[0].w = inPosition.x - u_DHM_I[0].w;
    Maux[1].w = inPosition.y - u_DHM_I[1].w;
    Maux[2].w = inPosition.z - u_DHM_I[2].w;

    mat4 aux = Maux * Rx * u_DHM ;
    vec4 Pos = vec4(aux[0].w, aux[1].w, aux[2].w, 1);

    mat4 modelViewMatrix = u_view * u_model;

    gl_Position = u_proj * modelViewMatrix * Pos;
    
    //========================================================

    vec4 P = modelViewMatrix * Pos;

    aux[0].w = 0;
    aux[1].w = 0;
    aux[2].w = 0;

    Maux[0].w = inNormal.x;
    Maux[1].w = inNormal.y;
    Maux[2].w = inNormal.z;

    mat4 Mnormal = Maux * aux;
    vec3 Normal = vec3(Mnormal[0].w, Mnormal[1].w, Mnormal[2].w);

    vs_out.N = mat3(modelViewMatrix) * Normal;
    vs_out.L = lightDirection - P.xyz;
    vs_out.V = -P.xyz;

    V_Colors = vec4(inColors,1);
}
