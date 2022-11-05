#version 450 core
out vec4 FragColor;

in vec2 textureCoord;
in mat4 pvm;
uniform sampler2D texture1;

void main()
{
    // FragColor = vec4(1.0f, 0.1f, 0.0f, 1.0f);
    // FragColor = vec4(textureCoord.x, textureCoord.y, 0.0f, 1.0f);
    FragColor = texture(texture1, textureCoord);
    // FragColor = vec4(textureCoord.x, textureCoord.y, 1.0, 1.0);

    // FragColor = projection * view * model * vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,
    //                 cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,
    //                 sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),
    //                 1.0);
}