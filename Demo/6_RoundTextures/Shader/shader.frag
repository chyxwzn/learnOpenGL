#version 410 core

in vec3 vertColor;
in vec2 texUV;
out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    vec4 texture1, texture2;
    texture1 = texture(tex1, texUV);
    texture2 = texture(tex2, texUV);
    color = mix(texture1, texture2, texture2.a * 0.2);
}
