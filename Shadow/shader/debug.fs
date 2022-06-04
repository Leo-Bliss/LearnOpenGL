#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float near;
uniform float far;
uniform sampler2D depthMap;

float linearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2.0 * near) / (far + near - z * (far - near));
}

void main()
{
	float depthValue = texture(depthMap, TexCoord).r;
	FragColor = vec4(vec3(linearizeDepth(depthValue) / far), 1.0); //perspective
	// FragColor = vec4(vec3(depthValue), 1.0); // orthographic
	
}