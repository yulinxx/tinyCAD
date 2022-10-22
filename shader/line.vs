#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

flat out vec3 startPos;
out vec3 vertPos;

void main()
{
    // gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vec4 pos = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    gl_Position = pos;

    startPos    = pos.xyz / pos.w;
    vertPos     = startPos;
}