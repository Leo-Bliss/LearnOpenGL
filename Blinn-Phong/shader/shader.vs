#version 330 core

layout(location = 0 ) in vec3 position;
layout(location = 1 ) in vec3 normal;
layout(location = 2 ) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}vs_out;

void main()
{
	gl_Position = projection * view * vec4(position, 1.0f);
	vs_out.Normal = normal;
	vs_out.TexCoord = texCoord;
	vs_out.FragPos = position;
}