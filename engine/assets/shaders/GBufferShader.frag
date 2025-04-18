#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 v_worldPos;
layout(location = 2) in vec3 v_normal;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outColor;
layout(location = 2) out vec4 outNormal;



void main()
{
    outColor = vec4(fragColor, 1.0);
    outPosition = vec4(v_worldPos, 1.0);
    outNormal = vec4(v_normal, 1.0);
}