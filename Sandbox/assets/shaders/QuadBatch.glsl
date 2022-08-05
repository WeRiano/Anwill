#shadertype vertex
#version 330 core

layout(location = 0) in vec2 v_Vertex;
layout(location = 1) in vec2 v_TexCoords;
layout(location = 2) in float v_TextureIndex;

out vec2 v_FragTexCoords;
flat out int v_FragTextureIndex;

uniform mat4 u_ViewProjMat;
//uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * vec4(v_Vertex, 0.0f, 1.0f);
    v_FragTexCoords = v_TexCoords;
    v_FragTextureIndex = int(v_TextureIndex);
}

#shadertype fragment
#version 330 core

in vec2 v_FragTexCoords;
flat in int v_FragTextureIndex;

layout(location = 0) out vec4 FragColor;

uniform sampler2D u_Textures[AW_MAX_TEXTURE_SLOTS];

void main()
{
    FragColor = texture(u_Textures[v_FragTextureIndex], v_FragTexCoords);
    //FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}