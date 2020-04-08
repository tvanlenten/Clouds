#version 430

in vec3 ModelPosition;
out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, ModelPosition);
}