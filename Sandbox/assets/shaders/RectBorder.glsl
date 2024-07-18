#shadertype vertex
#version 330 core

layout(location = 0) in vec2 a_Pos;

out vec4 v_Pos;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(a_Pos, 0.0f, 1.0f);
    v_Pos = vec4(a_Pos, 0.0f, 1.0);
}

#shadertype fragment
#version 330 core

in vec2 v_Pos;

layout(location = 0) out vec4 color;

uniform vec2 u_WidthHeight;

void main()
{
    float margin = 2.0f;
    vec2 center = u_WidthHeight * 1.0f/2.0f - margin;
    if(v_Pos.x > -center.x && v_Pos.x < center.x &&
       v_Pos.y > -center.y && v_Pos.y < center.y)
    {
        color = vec4(0.858f, 0.552f, 0.223f, 1.0f); // Inner
    } else {
        color = vec4(0.439f, 0.8f, 0.843f, 1.0f); // Outer
    }
}