#version 450
layout(input_attachment_index = 0, binding = 0) uniform subpassInput gPosition;
layout(input_attachment_index = 1, binding = 1) uniform subpassInput gColor;
layout(input_attachment_index = 2, binding = 2) uniform subpassInput gNormal;

layout(location = 0) out vec4 outColor;

vec4 directionalLight(vec3 normal, vec3 lightDir, vec3 objectColor, vec3 lightColor)
{
    vec3 ambient = lightColor;
    vec3 diffuse = max(dot(normal, lightDir), 0.0f) * lightColor;

    vec4 color = vec4((ambient + diffuse) * objectColor, 1.0f);
    return color;
}

void main() {
    vec3 normal = normalize(subpassLoad(gNormal).xyz);
    vec3 albedo = subpassLoad(gColor).rgb;

    outColor = directionalLight(normal, vec3(1.0, 1.0, 1.0), albedo, vec3(1.0, 1.0, 1.0));

    outColor = vec4(albedo, 1.0);
}
