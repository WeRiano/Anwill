#shadertype vertex
#version 330 core

layout(location = 0) in vec3 v_Pos;

//uniform mat4 u_ViewProjection;
//uniform mat4 u_Transform;

void main()
{
    gl_Position = vec4(v_Pos, 1.0);
}

#shadertype fragment
#version 330 core

layout(location = 0) out vec4 color;

//uniform vec4 u_Color;

void main()
{
    color = vec4(0.3f, 0.5f, 0.2f, 1.0f);
}