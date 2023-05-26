#version 330 core
out vec4 FragColor;

smooth in vec2 vTexCoord;

uniform sampler2D texture0;

void main()
{
    FragColor = texture(texture0, vTexCoord);
}