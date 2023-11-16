#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void makeShader(unsigned int * outpuShaderProgram)
{
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 iColor;"
    //"uniform float aMove;"
    "uniform float aCos;"
    "uniform float aSin;"
    "\n"
    "void main()\n"
    "{\n"
    //"  gl_Position = vec4(aPos.x, -1*aPos.y, aPos.z, 1.0);\n"
    //"  gl_Position = vec4(-1*aPos.y, aPos.x, aPos.z, 1.0);\n"
    //"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    //"  gl_Position = vec4(aPos.x + aMove, aPos.y, aPos.z, 1.0);\n"
    //"  gl_Position = vec4(aCos*(aPos.x - aPos.y), aSin*(aPos.x + aPos.y), aPos.z, 1.0f);\n"
    "  gl_Position = vec4(aPos.x*aCos - aPos.y*aSin, aPos.x*aSin + aPos.y*aCos, aPos.z, 1.0f);\n"
    "  iColor = aColor;"
    "}\0";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "failed compilling vertex shader\n" << infoLog << std::endl;
    }

    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 iColor;"
    "\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(iColor, 1.0f);\n"
    "}\0";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "failed compilling vertex shader\n" << infoLog << std::endl;
    }

    unsigned int programShader = glCreateProgram();
    glAttachShader(programShader, vertexShader);
    glAttachShader(programShader, fragmentShader);
    glLinkProgram(programShader);

    glGetProgramiv(programShader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "failed linking \n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    *outpuShaderProgram = programShader;
}

int main()
{
    glfwInit();
    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "invert", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed init glad" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int shaderProgram;
    makeShader(&shaderProgram);

    //float move = -1.5f;
    //float degre = 45;
    float simpleRad = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        /*
        move +=0.01f;
        if (move >= 1.5f)
            move = -1.5f;
        int vertexMoveLocation = glGetUniformLocation(shaderProgram, "aMove");
        glUniform1f(vertexMoveLocation, move);
        */
        int vertexCosLocation = glGetUniformLocation(shaderProgram, "aCos");
        int vertexSinLocation = glGetUniformLocation(shaderProgram, "aSin");

        //float radDegre = 3.14f/4;//degre*3.14f/360.0f;
        simpleRad += 0.005f;
        if (simpleRad >= 2*3.14f)
        {
            simpleRad = 0.0f;
        }
        float radDegre = simpleRad;

        glUniform1f(vertexCosLocation, std::cos(radDegre));
        glUniform1f(vertexSinLocation, std::sin(radDegre));
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}