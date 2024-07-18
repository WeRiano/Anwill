#shadertype vertex
#version 460 core

layout(location = 0) in vec2 a_Pos;

out vec2 v_Pos;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    v_Pos = a_Pos.xy;
    gl_Position = u_ViewProjMat * u_Transform * vec4(a_Pos, 0.0f, 1.0f);
}

#shadertype fragment
#version 460 core

in vec2 v_Pos;

layout(location = 0) out vec4 FragColor;

uniform mat4 u_Transform;
uniform vec3 u_Color;

void main()
{
    float distance = length(v_Pos);
    float edgeSoftness = fwidth(distance);
    float alpha = smoothstep(0.5f - edgeSoftness, 0.5f + edgeSoftness, distance);

    FragColor = vec4(u_Color, 1.0f - alpha);
}