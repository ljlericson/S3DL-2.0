#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

out vec2 texCoord;
out vec3 normal;
out vec3 crnt_pos;

uniform mat4 camMat;
uniform mat4 model;

void main()
{
    crnt_pos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = camMat * vec4(crnt_pos, 1.0);
    texCoord = aTex;
    normal = aNormal;
}