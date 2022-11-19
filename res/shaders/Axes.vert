#version 410 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColor;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

out vec4 V_Color;

void main()
{
    gl_Position = u_proj * u_view * u_model * inPosition;
    V_Color = inColor;
}
