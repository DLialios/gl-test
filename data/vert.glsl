#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 gTransform;

void main() {
    gl_Position = gTransform * vec4(position, 1.0f);
}
