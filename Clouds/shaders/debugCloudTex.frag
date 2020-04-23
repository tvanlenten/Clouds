#version 430
#define ERROR 1.0

in vec2 uv;
out vec4 FragColor;

uniform sampler3D cloudVolume;
uniform float slice;

void main()
{
	FragColor = vec4(texture(cloudVolume, vec3(uv, slice)).r);
}