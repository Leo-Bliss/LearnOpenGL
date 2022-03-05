#version 330 core
in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2; 
vec2 tempTexCoord; // 用于翻转第二个纹理的纹理坐标
void main()
{
	tempTexCoord = vec2(TexCoord.x, 1.0f-TexCoord.y);
	color = vec4(0.0f, 0.3f, 0.6f, 1.0f) * mix(texture(ourTexture1, TexCoord) ,texture(ourTexture2, tempTexCoord), 0.3);
};
