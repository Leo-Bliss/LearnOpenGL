#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
	vec4 texColor = texture(screenTexture, TexCoords);
	float redWeight = 0.2126;
	float greenWeight = 0.7152;
	float blueWeight = 1.0 - redWeight - greenWeight;
	float average = (redWeight * texColor.r + greenWeight * texColor.g + blueWeight * texColor.b) / 3.0;
	FragColor = vec4(vec3(average), 1.0);
}