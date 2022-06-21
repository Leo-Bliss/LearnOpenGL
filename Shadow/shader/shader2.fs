#version 330 core
out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool shadows;

float pcf(vec3 fragPos)
{
	vec3 fragToLight = fragPos - lightPos;
	float shadow = 0.0;
	float bias = 0.05;
	float samples = 4.0;
	float offset = 0.1;
	float steps = offset / (samples * 0.5);
	float currentDepth = length(fragToLight);
	for(float x = -offset; x < offset; x += steps)
	{
		for(float y = -offset; y < offset; y += steps)
		{
			for(float z = -offset; z < offset; z += steps)
			{
				float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r;
				closestDepth *= far_plane;
				if(currentDepth - bias > closestDepth)
					shadow += 1.0;
			}
		}
	}
	shadow /= (samples * samples * samples);
	return shadow;
}

float pcf2(vec3 fragPos) // reduce sample times: sample in perpendicular directions
{
	vec3 fragToLight = fragPos - lightPos;
	float currentDepth = length(fragToLight);
	vec3 sampleOffsetDirections[20] = vec3[]
	(
	   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
	   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	); 
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= far_plane;
		if(currentDepth - bias > closestDepth)
				shadow += 1.0;
	}
	shadow /= float(samples);
	return shadow;
}

float simpleShadow(vec3 fragPos)
{
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(depthMap, fragToLight).r;
	closestDepth *= far_plane;
	float currentDepth = length(fragToLight);
	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);
	return shadow;
}

float shadowCalculation(vec3 fragPos)
{
	//return simpleShadow(fragPos);

	//return pcf(fragPos);

	return pcf2(fragPos);
}

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoord).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(0.3f);
	// ambient
	vec3 ambient = 0.3 * color;
	// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	// calculate shadow
	float shadow = shadows ? shadowCalculation(fs_in.FragPos) : 0.0;
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0);

}