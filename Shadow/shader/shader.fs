#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosInLightSpace;
}fs_in;

uniform sampler2D floorTexture;
uniform sampler2D depthMap;
uniform vec3 lightPos;
uniform vec3 viewPos;


float calculateShadow(vec4 fragPosInLightSpace)
{
	// perform perspective divide
	vec3 projCoords = fragPosInLightSpace.xyz / fragPosInLightSpace.w;
	// transform NDC range from [-1, 1] to range [0, 1]
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow =  currentDepth > closestDepth ? 1.0 : 0.0;
	return shadow;
}
void main()
{
	vec3 color = texture(floorTexture, fs_in.TexCoord).rgb;
	vec3 lightColor = vec3(1.0);
	// ambient
	vec3 ambient = 0.15 * lightColor;
	// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	float shadow = calculateShadow(fs_in.FragPosInLightSpace);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	
	FragColor = vec4(lighting, 1.0);

}