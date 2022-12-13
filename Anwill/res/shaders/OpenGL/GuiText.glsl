#shadertype vertex
#version 330 core

layout(location = 0) in vec2 v_Vertex;
layout(location = 1) in vec2 v_TexCoords;
layout(location = 2) in float v_BitmapIndex;

out vec2 v_FragTexCoords;
flat out int v_FragBitmapIndex;

uniform mat4 u_ViewProjMat;
uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjMat * u_Transform * vec4(v_Vertex, 0.0f, 1.0f);
    v_FragTexCoords = v_TexCoords;
    v_FragBitmapIndex = int(v_BitmapIndex);
}

#shadertype fragment
#version 330 core

in vec2 v_FragTexCoords;
flat in int v_FragBitmapIndex;

out vec4 FragColor;

uniform mat4 u_Transform;
uniform sampler2D u_TextBitmaps[AW_MAX_TEXTURE_SLOTS];
uniform float u_CutoffWidth;

void main()
{
    vec2 centre = vec2(u_Transform[3][0], u_Transform[3][1]);
    if(gl_FragCoord.x > u_CutoffWidth) {
        discard;
    }

    vec3 textColor = vec3(1.0f, 1.0f, 1.0f);
    // We only care about the "x" element because our bitmap is monochrome/an 'alpha'
    // map (each pixel is 0-255, I think)
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(u_TextBitmaps[v_FragBitmapIndex],
    v_FragTexCoords).x);
    FragColor = vec4(textColor, 1.0f) * sampled;
}