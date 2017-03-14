#version 410 core

in vec3 vertColor;
out vec4 color;

void main()
{
    color = vec4(vertColor, 1.0f);
}
