#version 410 core

in vec3 vertColor;
in vec2 texUV;
out vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    color = mix(texture(texture0, texUV), texture(texture1, texUV), 0.8);
}
