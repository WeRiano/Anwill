#shadertype vertex
#version 460 core

layout(location = 0) in vec2 a_WorldPosition;
layout(location = 1) in vec2 a_LocalPosition;
layout(location = 2) in vec3 a_Color;

out vec2 v_WorldPosition;
out vec2 v_LocalPosition;
out vec3 v_Color;

uniform mat4 u_ViewProjMat;

void main()
{
    gl_Position = u_ViewProjMat * vec4(a_WorldPosition, 0.0f, 1.0f);
    v_LocalPosition = a_LocalPosition;
    v_Color = a_Color;
}

#shadertype fragment
#version 460 core

in vec2 v_WorldPosition;
in vec2 v_LocalPosition;
in vec3 v_Color;

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Textures[AW_MAX_FRAGMENT_SAMPLERS];

void main()
{
    float distance = length(v_LocalPosition);
    float edgeSoftness = fwidth(distance);
    float alpha = smoothstep(0.5f - edgeSoftness, 0.5f + edgeSoftness, distance);

    o_Color = vec4(v_Color, 1.0f - alpha);
}