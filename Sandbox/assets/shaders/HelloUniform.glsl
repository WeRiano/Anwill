#shadertype vertex
#version 330 core

layout(location = 0) in vec2 v_Pos;

uniform mat4 u_ProjMat;

void main()
{
    gl_Position = u_ProjMat * vec4(v_Pos, 0.0f, 1.0f);
}

#shadertype fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.5f, 0.3f, 0.2f, 1.0f);
}