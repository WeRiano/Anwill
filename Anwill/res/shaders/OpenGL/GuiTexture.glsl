#shadertype vertex
#version 330 core

layout(location = 0) in vec3 v_Pos;
layout(location = 1) in vec2 v_TexCoords;

out vec2 v_FragTexCoords;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(v_Pos, 1.0f);
    v_FragTexCoords = v_TexCoords;
}

#shadertype fragment
#version 330 core

in vec2 v_FragTexCoords;

layout(location = 0) out vec4 color;

uniform sampler2D u_TextureSampler;
uniform vec2 u_CutoffPos;

void main()
{
    if(gl_FragCoord.x > u_CutoffPos.x || gl_FragCoord.y < u_CutoffPos.y) {
        discard;
    }
    color = texture(u_TextureSampler, v_FragTexCoords);
}