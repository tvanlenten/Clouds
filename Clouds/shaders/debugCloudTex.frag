#version 430
#define ERROR 1.0

in vec2 uv;
out vec4 FragColor;

uniform sampler3D cloudVolume;
uniform float slice;
uniform int channel;

void main()
{
	float val;
	switch(channel)
	{
		case 0:
			val = texture(cloudVolume, vec3(uv, slice)).r;
			break;
		case 1:
			val = texture(cloudVolume, vec3(uv, slice)).g;
			break;
		case 2:
			val = texture(cloudVolume, vec3(uv, slice)).b;
			break;
		case 3:
			val = texture(cloudVolume, vec3(uv, slice)).a;
			break;
	}


	vec4 n = texture(cloudVolume, vec3(uv, slice));

	float cloud = n.x*0.5 + n.y*0.25 + n.z*0.125 + n.w*0.0625;



	FragColor = vec4(vec3(cloud), 1.0);
}