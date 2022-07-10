#shadertype vertex
#version 330 core

layout(location = 0) in vec3 v_Pos;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(v_Pos, 1.0f);
}

#shadertype fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_Radius;
uniform vec3 u_Color;
uniform vec3 u_CamPos;
uniform mat4 u_Transform;

void main()
{
    vec2 temp = vec2(gl_FragCoord.xy - (vec2(u_Transform[3][0], u_Transform[3][1]) + u_CamPos.xy));
    if (length(temp) > u_Radius)
    {
        discard;
    }

    if (length(temp) > (u_Radius - u_Radius/12.5))
    {
        color = vec4(0.3f, 0.2f, 0.5f, 1.0f);
    } else {
        color = vec4(u_Color, 1.0f);
    }
}