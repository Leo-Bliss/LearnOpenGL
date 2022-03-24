#version 330 core

out vec4 Fragcolor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;

};
uniform Material material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction); // 片段指向光源
	// 漫反射强度
	float diff = max(dot(normal, lightDir), 0.0f);
	// 镜面反射强度
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	// 影响因素叠加
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	return (ambient + diffuse + specular);
}

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// 漫反射强度
	float diff = max(dot(normal, lightDir), 0.0f);
	// 镜面反射强度
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	// 按照距离衰减
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// 影响因素叠加
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// 漫反射强度
	float diff = max(dot(normal, lightDir), 0.0f);
	// 镜面反射强度
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	// 按照距离衰减
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// 聚光平滑插值
	float theta = dot(-lightDir, normalize(light.direction));
	float intensity = (theta - light.outerCutOff) / (light.cutOff - light.outerCutOff);
	intensity = clamp(intensity, 0.0f, 1.0f); 

	// 影响因素叠加
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	Fragcolor = vec4(result, 1.0f);
}
