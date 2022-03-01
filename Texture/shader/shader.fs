#version 330 core
// in vec3 ourPosition;
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexCoord;
void main()
{
	color = texture(ourTexCoord, TexCoord) * vec4(ourColor, 1.0f); // ������ɫ�Ͷ�����ɫ���
};
