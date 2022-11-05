#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 bPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat4 pvm;
out vec2 textureCoord;

void main()
{
    // vec4 pos = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    mat4 pvm = projection * view * model;
    gl_Position =  pvm * vec4(aPos.x, aPos.y, aPos.z, 1.0); 
    // textureCoord = vec2(aPos.x, aPos.y);

    // vec4 t4 = projection * view * model * vec4(bPos.x, bPos.y, bPos.z, 1.0); 
    // textureCoord = vec2(t4.x, t4.y);

    // vec4 cPos =  pvm * vec4(bPos.x, bPos.y, bPos.z, 1.0); 
    vec4 cPos = vec4(bPos.x, bPos.y, bPos.z, 1.0); 
    textureCoord = vec2(bPos.x, bPos.y);
}