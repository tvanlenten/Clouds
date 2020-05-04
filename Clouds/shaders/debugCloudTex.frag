#version 430
#define ERROR 1.0

in vec2 uv;
out vec4 FragColor;

uniform sampler3D cloudVolume;
uniform float slice;
uniform int channel;

float remap(float x, float a, float b, float c, float d)
{
    return (((x - a) / (b - a)) * (d - c)) + c;
}

void main()
{
	float texVal = 0.0;
	switch(channel) {
		case 0:
			texVal = texture(cloudVolume, vec3(uv, slice)).r;
			break;
		case 1:
			texVal = texture(cloudVolume, vec3(uv, slice)).g;
			break;
		case 2:
			texVal = texture(cloudVolume, vec3(uv, slice)).b;
			break;
		case 3:
			texVal = texture(cloudVolume, vec3(uv, slice)).a;
			break;
		case 4:
			// Create cloud effects!
			float perlinWorley = texture(cloudVolume, vec3(uv, slice)).r;
    
			// worley fbms with different frequencies
			vec3 worley = texture(cloudVolume, vec3(uv, slice)).gba;
			float wfbm = worley.x * .625 +
						worley.y * .125 +
						worley.z * .25; 
			
			// cloud shape modeled after the GPU Pro 7 chapter
			float cloud = remap(perlinWorley, wfbm - 1., 1., 0., 1.);
			cloud = remap(cloud, .85, 1., 0., 1.); // fake cloud coverage
			texVal = cloud;
	}
	FragColor = vec4(vec3(texVal), 1.0);
}