#version 330 core

in vec3 ourColor;
out vec4 fragColor;

void main()
{
	fragColor = vec4(vec3(ourColor), 1.0);
}