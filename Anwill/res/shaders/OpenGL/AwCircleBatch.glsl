#shadertype vertex
#version 330 core

layout(location = 0) in vec2 a_Vertex;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_Centre;
layout(location = 4) in vec2 a_Radius;

out vec2 v_TexCoords;
out vec3 v_Color;
out vec2 v_Centre;
out vec2 v_Radius;

uniform mat4 u_ViewProjMat;

void main()
{
    gl_Position = u_ViewProjMat * vec4(a_Vertex, 0.0f, 1.0f);
    v_TexCoords = a_TexCoords;
    v_Color = a_Color;
    v_Centre = a_Centre;
    v_Radius = a_Radius;
}

#shadertype fragment
#version 330 core

in vec2 v_TexCoords;
in vec3 v_Color;
in vec2 v_Centre;
in vec2 v_Radius;

layout(location = 0) out vec4 FragColor;

uniform sampler2D u_Textures[AW_MAX_TEXTURE_SLOTS];

bool IsInsideEllipse(vec2 point, vec2 origin, vec2 radius)
{
    return (((point.x - origin.x)*(point.x - origin.x))*((radius.y * radius.y)) +
            ((point.y - origin.y)*(point.y - origin.y))*((radius.x * radius.x))) >=
            ((radius.x)*(radius.x)*(radius.y)*(radius.y));
}

void main()
{
    vec2 delta = vec2(gl_FragCoord.xy - (v_Centre));

    if (IsInsideEllipse(gl_FragCoord.xy, v_Centre, v_Radius))
    {
        discard;
    }

    FragColor = vec4(v_Color, 1.0f);
}