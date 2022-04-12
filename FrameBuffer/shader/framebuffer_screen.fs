#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
	vec4 texColor = texture(screenTexture, TexCoords);
	float average = (texColor.r + texColor.g + texColor.b) / 3.0;
	FragColor = vec4(vec3(average), 1.0);
}