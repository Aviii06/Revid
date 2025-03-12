#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 v_worldPos;
layout(location = 2) out vec3 v_normal;

void main()
{
    float x = gl_InstanceIndex % 200;
    float y = (gl_InstanceIndex/200) % 200;
    v_worldPos = (ubo.model *  vec4(inPosition, 1.0) + 0.25 * vec4(x, 0.0f, y, 0.0)).rgb;
    v_normal = normalize((ubo.model * vec4(inNormal, 0.0f)).rgb);
    gl_Position = ubo.proj * ubo.view * (vec4(v_worldPos, 1.0f));
    fragColor = inColor;
}