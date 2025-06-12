#version 330 core

out vec4 FragColor;

uniform vec4 light_col;

void main()
{
	FragColor = light_col;
}
