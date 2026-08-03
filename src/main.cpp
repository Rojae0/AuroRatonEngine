#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "triangle.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow *window);

constexpr int SCR_WIDTH = 1600;
constexpr int SCR_HEIGHT = 900;
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AuroRatonEngine", nullptr, nullptr);

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
    // 1280x720 해상도 화면 전체에 겹침 없이 넓게 배치된 삼각형 배열
    custom_triangle triangles[]
    {
        // 좌상단 영역 (큼직하고 비대칭적인 삼각형)
        custom_triangle(glm::vec3(-550, 150, 0), glm::vec3(-250, 300, 0), glm::vec3(-400, 50, 0)),
        // 우상단 영역 (가로로 길쭉한 형태)
        custom_triangle(glm::vec3(100, 280, 0), glm::vec3(580, 220, 0), glm::vec3(320, 80, 0)),
        // 화면 중앙 영역 (세로로 긴 비대칭 형태)
        custom_triangle(glm::vec3(-150, 120, 0), glm::vec3(150, -50, 0), glm::vec3(-80, -220, 0)),
        // 좌하단 영역 (넓게 벌어진 형태)
        custom_triangle(glm::vec3(-580, -100, 0), glm::vec3(-220, -320, 0), glm::vec3(-500, -300, 0)),
        // 우하단 영역 (크고 뾰족한 형태)
        custom_triangle(glm::vec3(200, -150, 0), glm::vec3(550, -120, 0), glm::vec3(400, -330, 0)),
        
        custom_triangle(glm::vec3(250, -240, 0), glm::vec3(350, -240, 0), glm::vec3(300, -100, 0))
    };
    
    glm::vec2 min_p = glm::vec2();
    glm::vec2 max_p = glm::vec2();
    for (custom_triangle triangle : triangles)
    {
        min_p.x = glm::min(min_p.x, triangle.get_minx());
        min_p.y = glm::min(min_p.y, triangle.get_miny());
        max_p.x = glm::max(max_p.x, triangle.get_maxx());
        max_p.y = glm::max(max_p.y, triangle.get_maxy());
    }
    
    //p % width, p / width
    for (int i = 0; i < bf.size(); i++)
    {
        int pixelX = i % SCR_WIDTH;
        int pixelY = i / SCR_WIDTH;

        glm::vec3 point(pixelX - SCR_WIDTH / 2, SCR_HEIGHT / 2 - pixelY, 0);
        
        if (point.x < min_p.x || point.y < min_p.y || point.x > max_p.x || point.y > max_p.y) continue;
        
        for (const auto& triangle : triangles)
        {
            float c1 = custom_triangle(triangle.p1, triangle.p2, point).cross().z;
            float c2 = custom_triangle(triangle.p2, triangle.p3, point).cross().z;
            float c3 = custom_triangle(triangle.p3, triangle.p1, point).cross().z;

            if ((c1 >= 0.f && c2 >= 0.f && c3 >= 0.f) || (c1 <= 0.f && c2 <= 0.f && c3 <= 0.f))
            {
                float area = custom_triangle(triangle.p1, triangle.p2, triangle.p3).cross().z;

                float w1 = c1 / area;
                float w2 = c2 / area;
                float w3 = c3 / area;
                
                uint32_t r = static_cast<uint32_t>(w1 * 255.0f);
                uint32_t g = static_cast<uint32_t>(w2 * 255.0f);
                uint32_t b = static_cast<uint32_t>(w3 * 255.0f);
                
                bf[i] = 255 << 24 | b << 16 | g << 8 | r;
            }
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
        ImGui::Image((ImTextureID)(intptr_t)tex, ImVec2(1280, 720));
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
