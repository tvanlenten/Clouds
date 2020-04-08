#version 430 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

out vec3 ModelPosition;

uniform mat4 projView;

void main()
{
	ModelPosition = vPosition;
    gl_Position = projView * vec4(vPosition, 1.0);
} 