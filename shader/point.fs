#version 450 core
out vec4 FragColor;

// uniform mat4 matColor = mat4(1.0f, 0.5f, 0.2f, 1.0f);
uniform vec4 color = vec4(1.0f, 0.5f, 0.2f, 1.0f);

void main()
{
    FragColor = color;
}