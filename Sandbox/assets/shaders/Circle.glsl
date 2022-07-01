#shadertype vertex
#version 330 core

layout(location = 0) in vec2 v_Pos;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(v_Pos, 0.0f, 1.0f);
}

#shadertype fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_Radius;
uniform vec3 u_Color;
uniform vec2 u_CentreCoords;
uniform vec2 u_CamPos;

void main()
{
    // TODO: Convert to uniforms for a general circle :)

    vec2 temp = vec2(gl_FragCoord.xy - (u_CentreCoords + u_CamPos));
    if (length(temp) > u_Radius)
    {
        discard;
    }

    if (length(temp) > (u_Radius - u_Radius/12.5))
    {
        color = vec4(0.3f, 0.2f, 0.5f, 1.0f);
    } else {
        color = vec4(u_Color, 1.0f);
    }
}