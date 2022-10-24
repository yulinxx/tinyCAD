#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 textureCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    pos = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    textureCoord = vec2(pos);
}