#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include <string>
#include <iostream>
#include <fstream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *source_v = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *source_f = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

struct Vertex {
    glm::vec3 pos;
    glm::vec3 nor;
    glm::vec2 tex;
};
typedef GLushort Index;

struct ObjData {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
};
ObjData objData;

std::string readFile(const char* filePath) {

    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


int main()
{
    Vertex vert;
    Index ind;

    vert.pos.x = 0.5f;
    vert.pos.y = -0.5f;
    vert.pos.z = 0.0f;

    vert.nor.x = 1.0f;
    vert.nor.y = 1.0f;
    vert.nor.z = 1.0f;

    vert.tex.x = 1.0f;
    vert.tex.y = 1.0f;
    ind = 1;
    objData.vertices.push_back(vert);
    objData.indices.push_back(ind);

    vert.pos.x = -0.5f;
    vert.pos.y = -0.5f;
    vert.pos.z = 0.0f;
    ind = 2;
    objData.vertices.push_back(vert);
    objData.indices.push_back(ind);

    vert.pos.x = 0.0f;
    vert.pos.y = 0.5f;
    vert.pos.z = 0.0f;
    ind = 3;
    objData.vertices.push_back(vert);
    objData.indices.push_back(ind);

    vert.pos.x = 0.3f;
    vert.pos.y = 0.3f;
    vert.pos.z = 0.0f;
    ind = 0;
    objData.vertices.push_back(vert);
    objData.indices.push_back(ind);


    glfwInit();
    // version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
//    std::string fragShaderStr = readFile("/Users/user/Desktop/3-2/OpenGL_pratice/OpenGL_1/shader.fs");
//    const char* source_f = fragShaderStr.c_str();

    GLuint shader_f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_f, 1, &source_f, NULL);
    glCompileShader(shader_f);


//    std::string vertShaderStr = readFile("/Users/user/Desktop/3-2/OpenGL_pratice/OpenGL_1/shader.vs");
//    const char* source_v = vertShaderStr.c_str();

    GLuint shader_v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_v, 1, &source_v, NULL);
    glCompileShader(shader_v);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, shader_v);
    glAttachShader(program, shader_f);
    glLinkProgram(program);


    GLuint vao, vbo;

    //generate and bind vertex array object(VAO): essential
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertex buffer object(VBO)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizei)objData.vertices.size() * sizeof(Vertex),
        objData.vertices.data(), GL_STATIC_DRAW);


    // element array buffer object
    GLuint eabo;
    glGenBuffers(1, &eabo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)objData.indices.size() * sizeof(Index),
        objData.indices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, pos));
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, nor));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, tex));
    
    
    // world Matrix
    glm::mat4 worldMatrix;
    GLint loc = glGetUniformLocation(program, "worldMat");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(worldMatrix));


    while (!glfwWindowShouldClose(window))
    {
        //Press ESC to quit
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //render the triangle
        glUseProgram(program);
        glBindVertexArray(vao);

        // GL_TRIANGLES 1
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // GL_TRIANGLES 2
//        glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_SHORT, 0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
