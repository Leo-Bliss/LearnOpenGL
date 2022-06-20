#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoord;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform bool reverse_normal;

void main()
{
	vs_out.FragPos = vec3(model * vec4(position, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * ((reverse_normal ? -1.0 : 1.0)* normal);
	vs_out.TexCoord = texCoord;
	gl_Position = projection * view * model * vec4(position, 1.0);
}