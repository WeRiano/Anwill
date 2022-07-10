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

void main()
{
    color = vec4(0.858f, 0.552f, 0.223f, 1.0f);
}