#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow *window);

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

float edgeFunction(const glm::vec3& a, const glm::vec3& b, const glm::vec3& p)
{
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

int main()
{
#pragma region Window Setting
    if (!glfwInit())
    {
        std::cout << "GLFW Init Failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Test", nullptr, nullptr);

    if (!window)
    {
        std::cout << "Window Create Failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD Init Failed\n";
        return -1;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
#pragma endregion
    
    std::vector<uint32_t> bf(SCR_WIDTH * SCR_HEIGHT);
    glm::vec3 vertices[]
    {
        glm::vec3(-160, -160, 0),
        glm::vec3(160, -160, 0),
        glm::vec3(0, 148, 0),
    };
    
    //p % width, p / width
    for (int i = 0; i < bf.size(); i++)
    {
        int pixelX = i % SCR_WIDTH;
        int pixelY = i / SCR_WIDTH;

        glm::vec3 point(pixelX - SCR_WIDTH / 2, SCR_HEIGHT / 2 - pixelY, 0);
        
        float c1 = glm::cross(vertices[1] - vertices[0], point - vertices[0]).z;
        float c2 = glm::cross(vertices[2] - vertices[1], point - vertices[1]).z;
        float c3 = glm::cross(vertices[0] - vertices[2], point - vertices[2]).z;
        
        if (c1 >= 0.f && c2 >= 0.f && c3 >= 0.f)
        {
            bf[i] = 0xffffffff;
        }
    }
    
    uint32_t tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, bf.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Framebuffer");
        ImGui::Image((ImTextureID)(intptr_t)tex, ImVec2(SCR_WIDTH, SCR_HEIGHT));
        ImGui::End();
        
        ImGui::Render();

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
