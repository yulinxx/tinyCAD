#version 450 core

uniform vec4 color = vec4(1.0f, 0.1f, 0.0f, 1.0f);

flat in vec3 startPos;
in vec3 vertPos;

out vec4 fragColor;

// uniform vec2  u_resolution;
// uniform uint  u_pattern = 0x18ff;
// uniform float u_factor = 2.0f;
vec2 u_resolution = vec2(1200,800);
uint  u_pattern = 0x18ff;   // 线型
// uint u_pattern = 0xf00f;
float u_factor = 1.f;

void main()
{
    vec2 dir = (vertPos.xy - startPos.xy) * u_resolution / 2.0;
    float dist = length(dir);
    
    uint bit = uint(round(dist / u_factor)) & 15U;
    if((u_pattern&(1U<<bit)) == 0U )
        discard;

    fragColor = color;
}