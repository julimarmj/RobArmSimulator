#version 410 core

layout(location = 0) out vec4 FragColor;

in vec4 V_Color;

void main()
{
	FragColor = V_Color;
}
