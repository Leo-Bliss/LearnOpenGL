#version 330 core
in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D screenTexture;

void main()
{
	vec3 color = texture(screenTexture, TexCoord).rgb;
	float grayscale = 0.216 * color.r + 0.715 * color.g + 0.0722 * color.b;
	FragColor = vec4(vec3(grayscale), 1.0);
}