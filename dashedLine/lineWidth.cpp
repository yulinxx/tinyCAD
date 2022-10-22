// https://stackoverflow.com/questions/60440682/drawing-a-line-in-modern-opengl

/////////////////////////////////////////////////////////////
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#define M_PI 3.1415926

std::string vertShader = R"(
#version 460

layout(std430, binding = 0) buffer TVertex
{
   vec4 vertex[]; 
};

uniform mat4  u_mvp;
uniform vec2  u_resolution;
uniform float u_thickness;

void main()
{
    int line_i = gl_VertexID / 6;
    int tri_i  = gl_VertexID % 6;

    vec4 va[4];
    for (int i=0; i<4; ++i)
    {
        va[i] = u_mvp * vertex[line_i+i];
        va[i].xyz /= va[i].w;
        va[i].xy = (va[i].xy + 1.0) * 0.5 * u_resolution;
    }

    vec2 v_line  = normalize(va[2].xy - va[1].xy);
    vec2 nv_line = vec2(-v_line.y, v_line.x);
    
    vec4 pos;
    if (tri_i == 0 || tri_i == 1 || tri_i == 3)
    {
        vec2 v_pred  = normalize(va[1].xy - va[0].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_pred.y, v_pred.x));

        pos = va[1];
        pos.xy += v_miter * u_thickness * (tri_i == 1 ? -0.5 : 0.5) / dot(v_miter, nv_line);
    }
    else
    {
        vec2 v_succ  = normalize(va[3].xy - va[2].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_succ.y, v_succ.x));

        pos = va[2];
        pos.xy += v_miter * u_thickness * (tri_i == 5 ? 0.5 : -0.5) / dot(v_miter, nv_line);
    }

    pos.xy = pos.xy / u_resolution * 2.0 - 1.0;
    pos.xyz *= pos.w;
    gl_Position = pos;
}
)";

std::string fragShader = R"(
#version 460

out vec4 fragColor;

void main()
{
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

GLuint CreateSSBO(std::vector<glm::vec4> &varray)
{
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo );
    glBufferData(GL_SHADER_STORAGE_BUFFER, varray.size()*sizeof(*varray.data()), varray.data(), GL_STATIC_DRAW); 
    return ssbo;
}


///////////////////////////////////////////////////////////
int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
    GLFWwindow *window = glfwCreateWindow( 800, 600, "lineWidth Test", nullptr, nullptr );
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

    GLint  loc_mvp  = glGetUniformLocation(nShaderProgID, "u_mvp");
    GLint  loc_res  = glGetUniformLocation(nShaderProgID, "u_resolution");
    GLint  loc_thi  = glGetUniformLocation(nShaderProgID, "u_thickness");


    glUseProgram(nShaderProgID);
    glUniform1f(loc_thi, 20.0);


    GLushort pattern = 0x18ff;
    GLfloat  factor  = 2.0f;

    std::vector<glm::vec4> varray;
    varray.emplace_back(glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
    varray.emplace_back(glm::vec4(1.0f, -1.0f, 0.0f, 1.0f));
    for (int u=0; u <= 90; u += 10)
    {
        double a = u*M_PI/180.0;
        double c = cos(a), s = sin(a);
        varray.emplace_back(glm::vec4((float)c, (float)s, 0.0f, 1.0f));
    }
    varray.emplace_back(glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f));
    for (int u = 90; u >= 0; u -= 10)
    {
        double a = u * M_PI / 180.0;
        double c = cos(a), s = sin(a);
        varray.emplace_back(glm::vec4((float)c-1.0f, (float)s-1.0f, 0.0f, 1.0f));
    }
    varray.emplace_back(glm::vec4(1.0f, -1.0f, 0.0f, 1.0f));
    varray.emplace_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    GLuint ssbo = CreateSSBO(varray);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    GLsizei N = (GLsizei)varray.size() - 2;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glm::mat4(project);
    int vpSize[2]{0, 0};
    while (!glfwWindowShouldClose(window))
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        if (w != vpSize[0] ||  h != vpSize[1])
        {
            vpSize[0] = w; vpSize[1] = h;
            glViewport(0, 0, vpSize[0], vpSize[1]);
            float aspect = (float)w/(float)h;
            project = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);
            glUniform2f(loc_res, (float)w, (float)h);
        }
             
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 modelview1( 1.0f );
        modelview1 = glm::translate(modelview1, glm::vec3(-0.6f, 0.0f, 0.0f) );
        modelview1 = glm::scale(modelview1, glm::vec3(0.5f, 0.5f, 1.0f) );
        glm::mat4 mvp1 = project * modelview1;
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, glm::value_ptr(mvp1));
        glDrawArrays(GL_TRIANGLES, 0, 6*(N-1));

        glm::mat4 modelview2( 1.0f );
        modelview2 = glm::translate(modelview2, glm::vec3(0.6f, 0.0f, 0.0f) );
        modelview2 = glm::scale(modelview2, glm::vec3(0.5f, 0.5f, 1.0f) );
        glm::mat4 mvp2 = project * modelview2;
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, glm::value_ptr(mvp2));
        glDrawArrays(GL_TRIANGLES, 0, 6*(N-1));
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}