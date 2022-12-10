#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texC;
out vec2 coord;
void main(void) {
    gl_Position = vec4(pos, 1.0);
    coord = texC;
}
