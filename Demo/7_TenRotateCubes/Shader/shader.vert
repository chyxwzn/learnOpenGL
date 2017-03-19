#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;
out vec2 texUV;

void main()
{
    gl_Position = projection * view * model * transform * vec4(position, 1.0f);
    texUV = vec2(uv.x, 1.0 - uv.y);
}
