#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "utility/Model.h"
#include "utility/Renderdoc.h"
#include "utility/Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow *window);

constexpr int SCR_WIDTH = 1600;
constexpr int SCR_HEIGHT = 900;
constexpr float PI = 3.1415926535f;

int main()
{
    InitRenderDoc();
    
#pragma region Window Setting
    if (!glfwInit())
    {
        std::cout << "GLFW Init Failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AuroRatonEngine", nullptr, nullptr);

    if (!window)
    {
        std::cout << "Window Create Failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "GLAD Init Failed\n";
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
#pragma endregion
    
    glEnable(GL_DEPTH_TEST);
    
    Shader modelShader("D:/01_Develope/CPP/AuroRatonEngine/src/shaders/Vert/model.vsh", "D:/01_Develope/CPP/AuroRatonEngine/src/shaders/Frag/model.fsh");
    
    float vertices[] =
    {
        // x, y
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,

        -1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    
    Model m("D:/01_Develope/CPP/AuroRatonEngine/model/dinosaur_obj/source/triceratops.obj");
    //Model m("D:/01_Develope/CPP/AuroRatonEngine/model/for my baby.obj");
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        //imgui 옵션
        RenderImGuiRenderDocMenu();
        
        ImGui::Render();

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(
            glm::vec3(0, 10, 10),   // 카메라 위치
            glm::vec3(0, 0, 0),   // 바라보는 지점
            glm::vec3(0, 1, 0)    // up 벡터
        );
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 100.0f
        );

        modelShader.use();
        modelShader.setMat4("model", model);
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);
        m.Draw(modelShader);
        

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
