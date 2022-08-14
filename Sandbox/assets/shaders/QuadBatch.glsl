#shadertype vertex
#version 330 core

layout(location = 0) in vec2 a_Vertex;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in float a_TextureIndex;

out vec2 v_TexCoords;
out vec3 v_Color;
flat out int f_TextureIndex;

uniform mat4 u_ViewProjMat;

void main()
{
    gl_Position = u_ViewProjMat * vec4(a_Vertex, 0.0f, 1.0f);
    v_TexCoords = a_TexCoords;
    v_Color = a_Color;
    f_TextureIndex = int(a_TextureIndex);
}

#shadertype fragment
#version 330 core

in vec2 v_TexCoords;
in vec3 v_Color;
flat in int f_TextureIndex;

layout(location = 0) out vec4 FragColor;

uniform sampler2D u_Textures[AW_MAX_TEXTURE_SLOTS];

void main()
{
    if(f_TextureIndex == -1) {
        FragColor = vec4(v_Color, 1.0f);
    } else {
        FragColor = texture(u_Textures[f_TextureIndex], v_TexCoords);
    }
}