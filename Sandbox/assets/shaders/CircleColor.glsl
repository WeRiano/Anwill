#shadertype vertex
#version 460 core

layout(location = 0) in vec3 v_Pos;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(v_Pos, 1.0f);
}

#shadertype fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

uniform mat4 u_Transform;
uniform vec3 u_Color;

void main()
{

    vec2 centre = vec2(u_Transform[3][0], u_Transform[3][1]);
    vec2 size = vec2( length(vec2(u_Transform[0][0], u_Transform[0][1])),
                      length(vec2(u_Transform[1][0], u_Transform[1][1])) );
    vec2 radii = size * 0.5f;

    vec2 normalized = (gl_FragCoord.xy - centre) / radii;
    float normDistance = length(normalized);
    float edgeSoftness = fwidth(normDistance);

    //vec3 color = vec3(normDistance);
    //vec3 color = vec3(step(1.0f, normDistance));
    vec3 color = vec3(smoothstep(1.0f - edgeSoftness, 1.0f + edgeSoftness, normDistance));
    color += u_Color;

    FragColor = vec4(color, 1.0f);
}