#shadertype vertex
#version 330 core

layout(location = 0) in vec2 a_Pos;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(a_Pos, 0.0f, 1.0f);
}

#shadertype fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec3 u_Color;

void main()
{
    FragColor = vec4(u_Color, 1.0f);
}