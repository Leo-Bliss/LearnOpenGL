#version 330 core
// in vec3 ourPosition;
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2; 
void main()
{
	color = mix(texture(ourTexture1, TexCoord) ,texture(ourTexture2, TexCoord), 0.2); //* vec4(ourColor, 1.0f); // 纹理颜色和顶点颜色混合
};
