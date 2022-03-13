#version 330 core

out vec4 color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// 环境光照
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// 漫反射光照
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); // 片段位置指向光源
	// 角度大于90时dot为负数，负数散射因子是没有意义的，所以取max
	float diff = max(dot(norm, lightDir), 0.0f); 
	vec3 duffuse = diff * lightColor;

	// 镜面光照
	float specularStrength = 0.5; // 镜面强度
	vec3 viewDir = normalize(viewPos -FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // 第一个需要光源指向片段位置的向量
	// 计算镜面亮度分量
	// 32是高光的发光值(Shininess): 一个物体的发光值越高，反射光的能力越强，散射得越少，高光点越小。
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// 光照影响相加
	vec3 result = (ambient + duffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
	
}
