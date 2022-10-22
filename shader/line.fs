#version 450 core

uniform vec4 color = vec4(1.0f, 0.1f, 0.0f, 1.0f);

// 平滑GL_SMOOTH、单调GL_FLAT着色
// 采用flat着色时，OpenGL将使用图元中某个顶点的颜色来渲染整个图元，
// 通常情况下会选择图元的第一个或最后一个顶点的颜色作为该图元的颜色。
// 在使用smooth着色时，OpenGL会独立的处理图元中各个顶点的颜色。
// 对于线段图元，线段上各点的颜色将根据两个顶点的颜色通过插值得到。
// 对于多边形图元，多边形内部区域的颜色将根据所有顶点的颜色插值得到。
// https://blog.csdn.net/hankern/article/details/122905085
flat in vec3 startPos;
in vec3 vertPos;

out vec4 fragColor;

uniform vec2  v2Resolution = vec2(1200,800);
uniform uint  nPattern = 0x18ff;
uniform float dFactor = 1.0f;

void main()
{
    // vec2 dir = (vertPos.xy - startPos.xy) * v2Resolution / 2.0;
    vec2 dir = (vertPos.xy - startPos.xy) * v2Resolution;
    float dist = length(dir);
    
    // uint bit = uint(round(dist / dFactor)) & 45U;
    uint bit = uint(round(dist / dFactor));

    // 使用discard会退出片段着色器，不执行后面的片段着色操作。片段也不会写入帧缓冲区。
    if((nPattern & (3U << bit)) == 0U )
        discard;

    fragColor = color;
}