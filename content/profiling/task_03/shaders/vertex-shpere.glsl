#version 460 core

layout (location = 0) in vec3 aPos;   /* vertex position          */
out vec3 oPos;                        /* input to fragment shader */

uniform float theta;    /* y rotatin angle */

void main()
{
    float gamma = 0.2f * theta;     /* x rotation angle */

    /* compute rotation matrices based on theta uniform & gamma */
    mat4 Ry = mat4( cos(theta), 0.0f, sin(theta), 0.0f,
                    0.0f,       1.0f, 0.0f,       0.0f,
                   -sin(theta), 0.0f, cos(theta), 0.0f,
                    0.0f,       0.0f, 0.0f,       1.0f);
    mat4 Rx = mat4(1.0f, 0.0f,        0.0f,       0.0f,
                   0.0f, cos(gamma), -sin(gamma), 0.0f,
                   0.0f, sin(gamma),  cos(gamma), 0.0f,
                   0.0f, 0.0f,        0.0f,       1.0f);

    /* fragment shader input is the rotated, but unscaled position */
    vec4 pos = Rx * Ry * vec4(aPos, 1.0f);
    oPos = pos.xyz;

    /* vertex position should be scaled to not occupy full window */
    gl_Position = vec4(0.8f * oPos.xyz, 1.0f);
}

