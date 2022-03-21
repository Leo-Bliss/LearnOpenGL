#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;

};

struct Light
{
	//vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 Fragcolor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	// 环境光照
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// 漫反射光照
	vec3 norm = normalize(Normal);
	vec3 lightDir =  normalize(-light.direction); //normalize(light.position - FragPos); // 片段位置指向光源
	// 角度大于90时dot为负数，负数散射因子是没有意义的，所以取max
	float diff = max(dot(norm, lightDir), 0.0f); 
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// 镜面光照
	float specularStrength = 0.5; // 镜面强度
	vec3 viewDir = normalize(viewPos -FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // 第一个需要光源指向片段位置的向量
	// 计算镜面亮度分量
	// 发光值(Shininess): 一个物体的发光值越高，反射光的能力越强，散射得越少，高光点越小。
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	// 光照影响相加
	vec3 result = ambient + diffuse + specular;
	Fragcolor = vec4(result, 1.0f);
	
}
