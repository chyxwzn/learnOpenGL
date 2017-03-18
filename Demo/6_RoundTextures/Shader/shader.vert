#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;

uniform mat4 transform;
out vec3 vertColor;
out vec2 texUV;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    vertColor = color;
    texUV = vec2(uv.x, 1.0 - uv.y);
}
