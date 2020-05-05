#version 430

in vec3 Position;
in vec3 Normal;
in vec2 UV;
out vec4 FragOut;

uniform vec3 cameraPos;
uniform vec3 sunDir;
uniform float sunPower;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	vec3 flatCol = vec3(0.164, 0.109, 0.074);

	vec3 col1 = mix(texture(tex0, UV*8.0).rgb, flatCol, 0.5);
	vec3 col0 = mix(texture(tex1, UV*8.0).rgb, vec3(0.278, 0.2, 0.121), 0.5);
	vec3 albedoCol = mix(col0, col1, smoothstep(0.8, 1.0, Normal.y));
	if(Position.y <= 2.0)
		discard;

	vec3 viewDir = normalize(-Position);
    vec3 refDir = reflect(-sunDir, Normal);

	vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

	float diff = max(dot(Normal, sunDir), 0.0);
	vec3 halfwayDir = normalize(sunDir + viewDir);
	float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.1) * spec;
    vec3 ambient = 0.1 * albedoCol;
    vec3 diffuse = diff * albedoCol;
        
    vec3 col = ambient + diffuse + specular;
	col = clamp(col, vec3(0.0), vec3(1.0));

	FragOut = vec4(col, 1.0);
}