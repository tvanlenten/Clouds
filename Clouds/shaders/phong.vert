#version 430 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 projView;
uniform mat4 model;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec3 Position;

void main()
{
	Normal = normalMatrix * vNormal;
	vec4 modelPos = model * vec4(vPosition, 1.0);
	Position = vec3(modelPos);
    gl_Position = projView * model * vec4(vPosition, 1.0);
} 