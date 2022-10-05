
/////////////////////////////////////////////////////////////
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>


std::string vertShader = R"(
#version 330

layout (location = 0) in vec3 inPos;

flat out vec3 startPos;
out vec3 vertPos;

uniform mat4 u_mvp;

void main()
{
    vec4 pos    = u_mvp * vec4(inPos, 1.0);
    gl_Position = pos;
    vertPos     = pos.xyz / pos.w;
    startPos    = vertPos;
}
)";

std::string fragShader = R"(
#version 330

flat in vec3 startPos;
in vec3 vertPos;

out vec4 fragColor;

uniform vec2  u_resolution;
uniform uint  u_pattern;
uniform float u_factor;

void main()
{
    vec2  dir  = (vertPos.xy-startPos.xy) * u_resolution/2.0;
    float dist = length(dir);

    uint bit = uint(round(dist / u_factor)) & 15U;
    if ((u_pattern & (1U<<bit)) == 0U)
        discard; 
    fragColor = vec4(1.0);
}
)";

GLuint CreateVAO(std::vector<glm::vec3> &varray)
{
    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, varray.size() * sizeof(*varray.data()), varray.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return vao;
}

///////////////////////////////////////////////////////////
int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
    GLFWwindow *window = glfwCreateWindow( 800, 600, "dashedLine", nullptr, nullptr );
    if ( window == nullptr )
        return 0;

    glfwMakeContextCurrent(window);
 
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    Shader shader(vertShader, fragShader);
    GLuint nShaderProgID = shader.ID;
    GLint loc_mvp     = glGetUniformLocation(nShaderProgID, "u_mvp");
    GLint loc_res     = glGetUniformLocation(nShaderProgID, "u_resolution");
    GLint loc_pattern = glGetUniformLocation(nShaderProgID, "u_pattern");
    GLint loc_factor  = glGetUniformLocation(nShaderProgID, "u_factor");

    glUseProgram(nShaderProgID);

    GLushort pattern = 0x18ff;
    GLfloat  factor  = 2.0f;
    glUniform1ui(loc_pattern, pattern);
    glUniform1f(loc_factor, factor);

    // 矩形
    glm::vec3 p0(-1.0f, -1.0f, 0.0f);
    glm::vec3 p1(1.0f, -1.0f, 0.0f);
    glm::vec3 p2(1.0f, 1.0f, 0.0f);
    glm::vec3 p3(-1.0f, 1.0f, 0.0f);

    std::vector<glm::vec3> varrayRec{ p0, p1, p1, p2, p2, p3, p3, p0 };
    GLuint vaoRec = CreateVAO(varrayRec);

    // 圆形
    std::vector<glm::vec3> varrayCircle;
    for (size_t i = 0; i <= 360; i += 8)
    {
        double a = i * 3.1415926 / 180.0;
        double c = cos(a), s = sin(a);
        varrayCircle.emplace_back(glm::vec3((float)c, (float)s, 0.0f));
    }
    GLuint vaoCircle = CreateVAO(varrayCircle);

    glm::mat4(project);
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    float aspect = (float)w/(float)h;
    project = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);
    glUniform2f(loc_res, (float)w, (float)h);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model_view_Rec( 1.0f );
        model_view_Rec = glm::translate(model_view_Rec, glm::vec3(-0.6f, 0.0f, 0.0f) );
        model_view_Rec = glm::scale(model_view_Rec, glm::vec3(0.5f, 0.5f, 1.0f) );
        glm::mat4 mvpRec = project * model_view_Rec;

        glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, glm::value_ptr(mvpRec));
        glBindVertexArray(vaoRec);
        glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)varrayRec.size());

        glm::mat4 model_view_Circle( 1.0f );
        model_view_Circle = glm::translate(model_view_Circle, glm::vec3(0.6f, 0.0f, 0.0f) );
        model_view_Circle = glm::scale(model_view_Circle, glm::vec3(0.5f, 0.5f, 1.0f) );
        glm::mat4 mvpCircle = project * model_view_Circle;

        glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, glm::value_ptr(mvpCircle));
        glBindVertexArray(vaoCircle);
        glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)varrayCircle.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}