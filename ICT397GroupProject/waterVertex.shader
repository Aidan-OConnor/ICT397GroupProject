#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 vertexColor;
out float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int maxHeight;
uniform int tileSize;

bool waterRising = true;

void main()
{
	if (aPos.y >= 5)
	{
		waterRising = false;
	}
	else if (aPos.y <= -5)
	{
		waterRising = true;
	}

	if (waterRising)
	{
		aPos.y += 100;
	}
	else
	{
		aPos.y -= 100;
	}

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aPos.x / tileSize, aPos.z / tileSize);
	height = aPos.y;
}