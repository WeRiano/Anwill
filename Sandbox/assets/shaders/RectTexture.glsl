#shadertype vertex
#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_FragTexCoords;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(a_Pos, 0.0f, 1.0f);
    v_FragTexCoords = a_TexCoords;
}

#shadertype fragment
#version 330 core

in vec2 v_FragTexCoords;

layout(location = 0) out vec4 color;

uniform sampler2D u_TextureSampler;

void main()
{
    color = texture(u_TextureSampler, v_FragTexCoords);
}