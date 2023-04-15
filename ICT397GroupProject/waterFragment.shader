#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 vertexColor;
in float height;

uniform sampler2D tex1;
//uniform sampler2D tex2;
//uniform sampler2D tex3

void main()
{
	FragColor = texture(tex1, TexCoord);

	//FragColor = vec4(1.0, 1.0, 0.0, 1.0);

	//FragColor = vertexColor;
}