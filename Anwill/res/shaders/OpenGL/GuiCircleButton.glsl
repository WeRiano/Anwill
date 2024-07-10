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

layout(location = 0) out vec4 o_Color;

uniform bool u_Hovering;
uniform bool u_Pressing;
uniform mat4 u_Transform;
uniform vec3 u_Color;
uniform vec3 u_HoverColor;
uniform vec3 u_PressColor;

void main()
{
    vec2 centre = vec2(u_Transform[3][0], u_Transform[3][1]);
    vec2 size = vec2( length(vec2(u_Transform[0][0], u_Transform[0][1])),
                      length(vec2(u_Transform[1][0], u_Transform[1][1])) );
    vec2 radii = size * 0.5f;


    vec2 normalized = (gl_FragCoord.xy - centre) / radii;
    float distance = length(normalized);
    float edgeSoftness = fwidth(distance);
    vec3 alpha = 1.0f - vec3(smoothstep(1.0f - edgeSoftness, 1.0 + edgeSoftness, distance));

    if(u_Pressing) {
        o_Color += vec4(u_PressColor, alpha);
    } else if(u_Hovering) {
        o_Color += vec4(u_HoverColor, alpha);
    } else {
        o_Color += vec4(u_Color, alpha);
    }
}