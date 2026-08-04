cmake_minimum_required(VERSION 3.20)

project(CPPAuroRatonEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)


add_executable(AuroRatonEngine
    src/main.cpp
        src/main.cpp
        src/triangle.h
        src/utility/Mesh.h
        src/utility/Mesh.cpp
        src/utility/Model.h
        src/utility/Model.cpp
        src/utility/Shader.h
        src/utility/Shader.cpp
        src/utility/stb_image.h
        src/utility/stb_image.cpp
)

target_link_libraries(AuroRatonEngine PRIVATE
    glfw
    imgui::imgui
    glad::glad
    glm::glm
    OpenGL::GL
)