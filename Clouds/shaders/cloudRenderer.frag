#version 430
#define MAX_RAY_STEPS 256
#define MIN_RAY_STEP_SIZE 0.001
#define ERROR 1.0

in vec2 uv;
out vec4 FragColor;

uniform mat4 invProjView;

uniform sampler3D cloudVolume;
uniform sampler2D prevDepth;

float min3(vec3 v) { return min(min(v.x, v.y), v.z); }
float max3(vec3 v) { return max(max(v.x, v.y), v.z); }
void getRay(inout vec3 ro, inout vec3 rd)
{
	vec2 uvPlane = uv * 2.0 - 1.0;
	vec4 rt = invProjView * vec4(uvPlane, -1.0, 1.0);
	ro = rt.xyz / rt.w;
	vec4 pt = invProjView * vec4(uvPlane, 1.0, 1.0);
	vec3 p = pt.xyz / pt.w;
	rd = normalize(p - ro);
}
void getDepth(inout float t, vec3 ro)
{
	float depth = texture(prevDepth, uv).r;
	vec3 ndc = vec3(uv, depth) * 2.0 - 1.0;
	vec4 dt = invProjView * vec4(ndc, 1.0);
	vec3 d = dt.xyz / dt.w;
	t = length(d - ro);
}
bool InBounds(vec3 coord, vec3 minB, vec3 maxB) { return !any(bvec3(step(minB, coord) - step(coord, maxB))); }


// constant step ray marching
float castRay(vec3 ro, vec3 rd, float tMin, float tMax, float stepSize, float cloudDensity, float scale)
{
	float t = tMin;
	float dt = tMax - tMin;
	
	float density = 0.0;
	while(t < tMax)
	{
		if(density >= 1.0)
			break;
		density += texture(cloudVolume, (ro + rd*t)*scale).r * cloudDensity;
		t += max(t*stepSize*0.5, stepSize);
	}

	return density;
}


void main()
{
	vec3 ro,rd;
	getRay(ro, rd);
	float tMax;
	getDepth(tMax, ro);

	vec3 minBound = vec3(-10.0);
	vec3 maxBound = vec3(10.0);

	vec3 pos0 = (minBound - ro) / rd;
	vec3 pos1 = (maxBound - ro) / rd;
	float boxTmin = max3(min(pos0, pos1));
	float boxTmax = min3(max(pos0, pos1));

	if(boxTmin > boxTmax || boxTmax < 0.0)
		discard;

	float tMin = (InBounds(ro, minBound, maxBound))? 0.0 : boxTmin;
	tMax = min(boxTmax, tMax);

	float density = castRay(ro, rd, tMin, tMax, 0.01, 0.1, 0.1);

	
	FragColor = vec4(1.0, 1.0, 1.0, density);

}