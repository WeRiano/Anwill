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

uniform bool u_Hovering;
uniform bool u_Pressing;
uniform mat4 u_Transform;
uniform vec2 u_CutoffPos;

void main()
{
    vec2 centre = vec2(u_Transform[3][0], u_Transform[3][1]);
    vec2 size = vec2( length(vec2(u_Transform[0][0], u_Transform[0][1])),
                      length(vec2(u_Transform[1][0], u_Transform[1][1])) );

    if(gl_FragCoord.x > u_CutoffPos.x || gl_FragCoord.y < u_CutoffPos.y) {
        discard;
    }

    if(u_Pressing) {
        color = vec4(0.1f, 0.40f, 1.0f, 1.0f);
    } else if(u_Hovering) {
        color = vec4(0.3f, 0.48f, 1.0f, 1.0f);
    } else {
        color = vec4(0.0f, 0.22f, 0.8f, 1.0f);
    }
}