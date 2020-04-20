#version 430

in vec3 Position;
in vec3 Normal;
in vec2 UV;
out vec4 FragOut;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec4 color;

uniform samplerCube skybox;

void main()
{
	vec3 albedoCol = mix(vec3(0.164, 0.109, 0.074), vec3(0.086, 0.192, 0.094), smoothstep(0.3, 0.6, Normal.y));


	vec3 lightDir = normalize(lightPos - Position);
	vec3 viewDir = normalize(-Position);
    vec3 refDir = reflect(-lightDir, Normal);

	vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
	//vec3 skyboxCol = texture(skybox, R).rgb;

	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.1) * spec;
    vec3 ambient = 0.35 * albedoCol;
    vec3 diffuse = diff * albedoCol;
        
    vec3 col = ambient + diffuse + specular;
	col = clamp(col, vec3(0.0), vec3(1.0));

	FragOut = vec4(col, color.w);
}