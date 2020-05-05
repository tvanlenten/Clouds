#version 430
#define MAX_RAY_STEPS 256
#define MIN_RAY_STEP_SIZE 0.001
#define ERROR 1.0
#define MAX_T 30.0

in vec2 uv;
out vec4 FragColor;

uniform mat4 invProjView;

uniform sampler3D cloudVolume;
uniform samplerCube skybox;
uniform sampler2D prevColorTex;
uniform sampler2D prevDepthTex;

// DEBUG UNIFORMS
uniform float xScale;
uniform float yScale;
uniform float zScale;
uniform float cloudDefinition;
uniform float cloudTrim;
uniform float cloudHeight;
uniform float cloudSlice;

uniform vec3 lightDir;
uniform float stepSize;

float min3(vec3 v) { return min(min(v.x, v.y), v.z); }
float max3(vec3 v) { return max(max(v.x, v.y), v.z); }
float remap(float x, float a, float b, float c, float d) { return (((x - a) / (b - a)) * (d - c)) + c; }

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
	float depth = texture(prevDepthTex, uv).r;
	vec3 ndc = vec3(uv, depth) * 2.0 - 1.0;
	vec4 dt = invProjView * vec4(ndc, 1.0);
	vec3 d = dt.xyz / dt.w;
	t = (depth == 1.0)? MAX_T : length(d - ro);
}

bool InBounds(vec3 coord, vec3 minB, vec3 maxB) { return !any(bvec3(step(minB, coord) - step(coord, maxB))); }

float getCloud(vec3 p, vec3 scale, float cloudDensity)
{
	vec4 den = texture(cloudVolume, p * vec3(xScale, yScale, zScale));
	float perlinWorley = smoothstep(cloudDefinition, 1.0, den.r);
	// worley fbms with different frequencies
	vec3 worley = den.gba;
	float wfbm = worley.x * .625 + // 0.415
				worley.y * .125 + // 0.640
				worley.z * .25; // 0.125
	// cloud shape modeled after the GPU Pro 7 chapter
	float cloud = remap(perlinWorley, wfbm - 1.0, 1.0, 0.0, 1.0);
	cloud = remap(cloud, cloudTrim, 1.0, 0.0, 1.0); // fake cloud coverage

	// vec4 den = texture(cloudVolume, p * vec3(0.07,0.1,0.1));
	// float f = smoothstep(0.6, 1.0, den.x)*0.5 + den.y*0.25 + den.z*0.125 + den.w*0.0625;

	return smoothstep(1.0, 0.0, abs((p.y - cloudHeight)*cloudSlice)) * cloud * cloudDensity; //clamp(-abs(p.y - 6.0) + 4.0*f, 0.0, 1.0 );
}

vec3 castRayIQ(vec3 ro, vec3 rd, float tMin, float tMax, float stepSize, float cloudDensity, vec3 scale, vec3 prevCol)
{
	float t = tMin;
	vec4 sum = vec4(0.0);

	vec3 lightCol = vec3(1.0);//vec3(1.0,0.6,0.3);
	vec3 skyCol = vec3(0.25,0.3,0.35);
	vec3 cloudCol = vec3(1.0,0.95,0.8);
	float lightPower = 1.0;

	while(t < tMax)
	{
		vec3 pos = ro + t*rd;
		float den = getCloud(pos, scale, cloudDensity)*cloudDensity;
		if( den>0.01 )
		{

			float cc = getCloud(pos+0.3*lightDir, scale, cloudDensity);
			float dif = clamp((den - cc)*1.5, 0.0, 1.0 );

			//col.xyz = mix( col.xyz, prevCol, 1.0-exp(-0.003*t*t) );
	
			vec3 lin = vec3(lightPower) + lightPower*lightCol*dif;
			vec4 col = vec4(mix(cloudCol, skyCol, den), den);
			col.xyz *= lin;

			col.w *= 0.1;
			col.rgb *= col.a;
			sum += col*(1.0-sum.a);
		}
		t += max(stepSize,stepSize*0.5*t);
	}

	vec4 res = clamp(sum, 0.0, 1.0);
	return prevCol*(1.0-res.w) + res.xyz;
}

//note: from https://www.shadertoy.com/view/4djSRW
// This set suits the coords of of 0-1.0 ranges..
#define MOD3 vec3(443.8975,397.2973, 491.1871)
float hash12(vec2 p)
{
	vec3 p3  = fract(vec3(p.xyx) * MOD3);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

void main()
{
	vec3 ro,rd;
	getRay(ro, rd);
	float tMax;
	getDepth(tMax, ro);

	vec3 prevColor = texture(prevColorTex, uv).rgb;

	vec3 minBound = vec3(-40.0, 4.0, -40.0);
	vec3 maxBound = vec3(40.0, 12.0, 40.0);

	vec3 pos0 = (minBound - ro) / rd;
	vec3 pos1 = (maxBound - ro) / rd;
	float boxTmin = max3(min(pos0, pos1));
	float boxTmax = min3(max(pos0, pos1));

	if(boxTmin > boxTmax || boxTmax < 0.0)
	{
		FragColor = vec4(prevColor, 1.0);
		return;
	}


	float tMin = (InBounds(ro, minBound, maxBound))? 0.0 : boxTmin;
	tMax = min(boxTmax, tMax);

	//ro = ro + rd*tMin;
	//tMax = tMax - tMin;

	//float density = castRay(ro, rd, tMin, tMax, 0.06, 0.5, 1.0 / vec3(10.0, 3.0, 10.0));

	//float transmittance = clamp(exp(-density), 0.0, 1.0);

	vec3 col = castRayIQ(ro, rd, tMin + hash12(uv)*stepSize*5.0, tMax, stepSize, 1.0, vec3(1.0), prevColor); //1.0 / vec3(10.0, 8.0, 10.0)

	FragColor = vec4(col,1.0);;

}