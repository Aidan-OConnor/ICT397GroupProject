#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 vertexColor;
in float height;

uniform sampler2D tex1;

void main()
{
	FragColor = texture(tex1, TexCoord);
}