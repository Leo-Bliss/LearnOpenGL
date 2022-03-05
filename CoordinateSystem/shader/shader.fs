#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2; 
vec2 tempTexCoord; // 用于翻转第二个纹理的纹理坐标
void main()
{
	tempTexCoord = vec2(TexCoord.x, 1.0f-TexCoord.y);
	color = mix(texture(ourTexture1, TexCoord) ,texture(ourTexture2, tempTexCoord), 0.3) ;//* vec4(ourColor, 1.0f); // 纹理颜色和顶点颜色混合
};
