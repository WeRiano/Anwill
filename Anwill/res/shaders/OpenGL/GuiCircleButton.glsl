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
uniform vec3 u_Color;
uniform vec3 u_HoverColor;
uniform vec3 u_PressColor;

bool IsOutsideEllipse(vec2 point, vec2 origin, vec2 radius)
{
    return (((point.x - origin.x)*(point.x - origin.x))*((radius.y * radius.y)) +
    ((point.y - origin.y)*(point.y - origin.y))*((radius.x * radius.x))) >=
    ((radius.x)*(radius.x)*(radius.y)*(radius.y));
}

void main()
{
    vec2 centre = vec2(u_Transform[3][0], u_Transform[3][1]);
    vec2 size = vec2( length(vec2(u_Transform[0][0], u_Transform[0][1])),
                      length(vec2(u_Transform[1][0], u_Transform[1][1])) );
    vec2 radius = vec2(size.x * 0.5f, size.y * 0.5f);

    if(gl_FragCoord.x > u_CutoffPos.x || gl_FragCoord.y < u_CutoffPos.y) {
        discard;
    }

    if (IsOutsideEllipse(gl_FragCoord.xy, centre, radius))
    {
        discard;
    }

    if(u_Pressing) {
        color = vec4(u_PressColor, 1.0f);
    } else if(u_Hovering) {
        color = vec4(u_HoverColor, 1.0f);
    } else {
        color = vec4(u_Color, 1.0f);
    }
}