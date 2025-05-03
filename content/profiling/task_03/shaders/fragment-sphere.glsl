#version 460 core

out vec4 FragColor;
in  vec3 oPos;          /* output from vertex shader */

void main()
{
    FragColor = vec4(oPos.z < 0.0f ? -0.3 * oPos.z : 0.0f,
                     0.0f,
                     oPos.z > 0.0f ? oPos.z : 0.0f,
                     1.0f);
}

