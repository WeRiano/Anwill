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

uniform vec3 u_Color;
uniform vec2 u_CutoffPos;

void main()
{
    if(gl_FragCoord.x > u_CutoffPos.x || gl_FragCoord.y < u_CutoffPos.y) {
        discard;
    }
    color = vec4(u_Color, 1.0f);
}