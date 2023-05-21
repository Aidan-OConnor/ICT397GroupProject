#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 vertexColor;
in float height;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform float t1Height = -10;
uniform float t2Height = 10;
uniform float t3Height = 50;

vec4 blendedTex;

void main()
{
	if (height < t1Height)
	{
		FragColor = texture(tex1, TexCoord);
	}
	else if (height < t2Height)
	{
		vec4 temp1 = texture(tex1, TexCoord);
		vec4 temp2 = texture(tex2, TexCoord);
		float delta = t2Height - t1Height;
		float factor = (height - t1Height) / delta;
		blendedTex = mix(temp1, temp2, factor);
		FragColor = blendedTex;
	}
	else
	{
		vec4 temp1 = texture(tex2, TexCoord);
		vec4 temp2 = texture(tex3, TexCoord);
		float delta = t3Height - t2Height;
		float factor = (height - t2Height) / delta;
		blendedTex = mix(temp1, temp2, factor);
		FragColor = blendedTex;
	}
}