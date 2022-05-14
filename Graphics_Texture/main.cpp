#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include <string>
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *source_v = "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aColor;\n"
    "layout(location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";

const char *source_f = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, TexCoord);\n"
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
    vert.pos.y = 0.5f;
    vert.pos.z = 0.0f;

    vert.nor.x = 1.0f;
    vert.nor.y = 1.0f;
    vert.nor.z = 1.0f;

    vert.tex.x = 2.0f;
    vert.tex.y = 2.0f;
    ind = 0;
    objData.vertices.push_back(vert);
    objData.indices.push_back(ind);

    vert.pos.x = 0.5f;
    vert.pos.y = -0.5f;
    vert.pos.z = 0.0f;
    
    vert.tex.x = 2.0f;
    vert.tex.y = 0.0f;
    ind = 1;
    objData.vertices.push_back(vert);
    objData.indices.push_back(ind);

    vert.pos.x = -0.5f;
    vert.pos.y = -0.5f;
    vert.pos.z = 0.0f;
    
    vert.tex.x = 0.0f;
    vert.tex.y = 0.0f;
    ind = 2;
    objData.vertices.push_back(vert);
    objData.indices.push_back(ind);

    vert.pos.x = -0.5f;
    vert.pos.y = 0.5f;
    vert.pos.z = 0.0f;
    
    vert.tex.x = 0.0f;
    vert.tex.y = 2.0f;
    ind = 3;
    objData.vertices.push_back(vert);
    objData.indices.push_back(ind);

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

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


    GLuint vao, vbo, eabo;

    //generate and bind vertex array object(VAO): essential
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &eabo);

    glBindVertexArray(vao);

    // vertex buffer object(VBO)
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizei)objData.vertices.size() * sizeof(Vertex),
        objData.vertices.data(), GL_STATIC_DRAW);
    

    // element array buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)objData.indices.size() * sizeof(Index),
    //    objData.indices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // GL program for vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, nor));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, tex));


    // load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("/Users/user/Desktop/3-2/OpenGL_pratice/OpenGL_1/OpenGL_1/wall.jpg", &width, &height, &nrChannels, 0);
    // texture

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    /////////////   여기 아래부터 짜보기    //////////////////////////
    // create texture and generate mipmaps (use glTexImage2D, use GL_RGB instead of GL_RGBA)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    // set the texture wrapping parameters: texture wrapping to GL_REPEAT (to both s and t)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // set texture filtering parameters (both linear to min and mag filter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /////////////////////////////////////////////////////////////////


    // free image
    stbi_image_free(data);

    glUseProgram(program);
    // uniform ¿ßƒ° ¡ˆ¡§
    glUniform1i(glGetUniformLocation(program, "colorMap"), 0);


    while (!glfwWindowShouldClose(window))
    {
        //Press ESC to quit
        processInput(window);
        // ƒ√∑Ø πˆ∆€ ¡ˆøÔ ªˆªÛ ¡ˆ¡§
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // ƒ√∑Ø πˆ∆€ ¡ˆøÏ±‚
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture1);

        //render the triangle
        glUseProgram(program);
        glBindVertexArray(vao);

        // index ªÁøÎ
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // ƒ√∑Ø πˆ∆€ ±≥√º
        glfwSwapBuffers(window);
        // ¿Ã∫•∆Æ πﬂª˝ »Æ¿Œ (≈∞∫∏µÂ ¿‘∑¬¿Ã≥™ ∏∂øÏΩ∫ ¿Ãµø)
        glfwPollEvents();
        
    }
    // ¡æ∑·
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
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    // º¯º≠¥Î∑Œ, øﬁ¬  æ∆∑° ∏º≠∏Æ ¿ßƒ° (x,y), ≥ ∫Ò ≥Ù¿Ã (w, h)
    glViewport(0, 0, width, height);
}
