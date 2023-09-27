#version 330
#shader vertex
layout(location = 0) in vec3 aPos;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main() {
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
#shader frag
layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
