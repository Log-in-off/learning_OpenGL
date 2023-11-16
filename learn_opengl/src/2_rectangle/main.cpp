#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include <iostream>

void framebuffer_size_calback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0,  width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW windows" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0,  800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_calback);

    
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
  //  "out vec4 ColorT;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  //  "  ColorT = vec4(1.0f, 0.9f, 0.1f, 1.0f);\n"
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
        std::cout << "ERROR::SHADER:VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    const char* fragmetnShaderSourse = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;"
  //  "in vec4 ColorT;"
    "\n"
    "void main()\n"
    "{\n"
    "  FragColor = ourColor;\n"
    //"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmetnShaderSourse, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT:COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        std::cout << "ERROR::PROGRAM::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


 // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
  
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    /*
        float read = 0;
        float grean = 0;
        float blue = 0;
    */

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        float greanValue = (std::sin(time)/2.0f + 0.5f);
/*
        read +=0.1;
        if (read > 1)
        {
            read = 0;
            grean += 0.1;
            if (grean > 1)
            {
                grean = 0;
                blue += 0.1;
                if (blue > 1)
                blue = 0;
            }
        }
*/


        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greanValue, 0.0f, 1.f);
        //glUniform4f(vertexColorLocation, grean, read, blue, 1.0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}