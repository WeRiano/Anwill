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

#define MAX_WALL_SEGMENTS 100

struct PLine {
    vec2 p1, p2;
};

layout(location = 0) out vec4 color;

uniform vec2 u_PlayerPos;
uniform vec2 u_LookDir;
uniform float u_FoVAngleDeg;
uniform vec2 u_CamPos;
uniform PLine u_WallSegs[100];

bool wallColl()
{
    vec2 p1 = u_PlayerPos;
    vec2 p2 = vec2(gl_FragCoord.xy - u_CamPos); // This might be wrong

    for(int i = 0; i < MAX_WALL_SEGMENTS; i++) {
        vec2 p3 = u_WallSegs[i].p1;
        vec2 p4 = u_WallSegs[i].p2;

        float t = ((p1.x-p3.x)*(p3.y-p4.y) - (p1.y-p3.y)*(p3.x-p4.x))/((p1.x-p2.x)*(p3.y-p4.y) - (p1.y-p2.y)*(p3.x-p4.x));
        float u = ((p1.x-p3.x)*(p1.y-p2.y) - (p1.y-p3.y)*(p1.x-p2.x))/((p1.x-p2.x)*(p3.y-p4.y) - (p1.y-p2.y)*(p3.x-p4.x));
        if( (t >= 0.0f && t <= 1.0f) && (u >= 0.0f && u <= 1.0f)) {
            // Intersection
            return true;
        }
    }
    return false;
}

void main()
{
    mat2 rotMat;
    rotMat[0][0] = cos(radians(u_FoVAngleDeg));
    rotMat[0][1] = sin(radians(u_FoVAngleDeg));
    rotMat[1][0] = -sin(radians(u_FoVAngleDeg));
    rotMat[1][1] = cos(radians(u_FoVAngleDeg));

    vec2 fragDirVec = normalize(vec2(gl_FragCoord.xy - (u_PlayerPos + u_CamPos)));
    vec2 leftFoVDirVec = normalize(rotMat * u_LookDir);
    vec2 rightFovDirVec = normalize(rotMat * u_LookDir);
    float leftDot = dot(u_LookDir, leftFoVDirVec);
    float rightDot = dot(u_LookDir, rightFovDirVec);
    float fragDot = dot(u_LookDir, fragDirVec);

    if ( (fragDot > leftDot && fragDot > rightDot) && !wallColl() ) {
        color = vec4(0.9f, 0.9f, 0.5f, 1.0f);
    } else {
        color = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    }
}