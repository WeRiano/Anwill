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

uniform mat4 u_Transform;
uniform bool u_Selected;

void main()
{
    vec2 centre = vec2(u_Transform[3][0], u_Transform[3][1]);
    vec2 size = vec2( length(vec2(u_Transform[0][0], u_Transform[0][1])),
                      length(vec2(u_Transform[1][0], u_Transform[1][1])) );
    // Can get width and size in the same way (I think!)
    float borderSize = AW_GUI_WINDOW_BORDER_SIZE;
    float headerSize = AW_GUI_WINDOW_HEADER_SIZE;
    float radius = borderSize;
    vec2 tr = vec2(centre.x + size.x / 2 - borderSize, centre.y + size.y / 2 - borderSize);
    vec2 br = vec2(centre.x + size.x / 2 - borderSize, centre.y - size.y / 2 + borderSize);
    vec2 bl = vec2(centre.x - size.x / 2 + borderSize, centre.y - size.y / 2 + borderSize);
    vec2 tl = vec2(centre.x - size.x / 2 + borderSize, centre.y + size.y / 2 - borderSize);

    // Smooth corners
    if ((gl_FragCoord.x > tr.x && gl_FragCoord.y > tr.y && length(gl_FragCoord.xy - tr) > radius) ||
        (gl_FragCoord.x > br.x && gl_FragCoord.y < br.y && length(gl_FragCoord.xy - br) > radius) ||
        (gl_FragCoord.x < bl.x && gl_FragCoord.y < bl.y && length(gl_FragCoord.xy - bl) > radius) ||
        (gl_FragCoord.x < tl.x && gl_FragCoord.y > tl.y && length(gl_FragCoord.xy - tl) > radius) ) {
        discard;
    } else if(gl_FragCoord.y > centre.y + size.y / 2 - headerSize) {
        // Header
        if(u_Selected) {
            color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        } else {
            color = vec4(0.1f, 0.1f, 0.1f, 1.0f);
        }
    } else {
        color = vec4(0.1f, 0.1f, 0.1f, 0.7f);
    }
}