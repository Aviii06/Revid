#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main()
{
    int x = gl_InstanceIndex / 100;
    int y = (gl_InstanceIndex / 10) % 10;
    int z = gl_InstanceIndex % 10;
    gl_Position = ubo.proj * ubo.view * (ubo.model *  vec4(inPosition, 1.0) + 2 * vec4(x, y, z, 0.0));
//    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}