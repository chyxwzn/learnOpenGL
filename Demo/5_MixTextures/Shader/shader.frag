#version 410 core

in vec3 vertColor;
in vec2 texUV;
out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    color = mix(texture(tex1, texUV), texture(tex2, texUV), 0.8);
}
