#version 330 core
out vec4 FragColor;
in VS_OUT
{
	vec2 TexCoord;
}fs_in;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{
	if(gl_FrontFacing)
		FragColor = texture(frontTexture, fs_in.TexCoord);
	else
		FragColor = texture(backTexture, fs_in.TexCoord);
}