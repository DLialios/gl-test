#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 gTransform;

smooth out vec4 Color;

void main() {
    gl_Position = gTransform * vec4(position, 1.0f);
    Color = vec4(color, 1.0f);
}
