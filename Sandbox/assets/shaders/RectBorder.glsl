#shadertype vertex
#version 330 core

layout(location = 0) in vec3 v_Pos;

out vec4 Position;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(v_Pos, 1.0f);
    Position = vec4(v_Pos, 1.0);
}

#shadertype fragment
#version 330 core

in vec4 Position;

layout(location = 0) out vec4 color;

uniform vec2 u_WidthHeight;

void main()
{
    float margin = 2.0f;
    vec2 center = u_WidthHeight * 1.0f/2.0f - margin;
    if(Position.x > -center.x && Position.x < center.x &&
       Position.y > -center.y && Position.y < center.y)
    {
        color = vec4(0.858f, 0.552f, 0.223f, 1.0f); // Inner
    } else {
        color = vec4(0.439f, 0.8f, 0.843f, 1.0f); // Outer
    }
}