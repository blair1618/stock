cmake_minimum_required(VERSION 3.2)

project(stock)

add_subdirectory(deps)

set(STOCK_SOURCES
    src/debug/DebugDraw.hpp
    src/debug/DebugDraw.cpp
    src/gl/Error.hpp
    src/gl/Error.cpp
    src/gl/Framebuffer.hpp
    src/gl/Framebuffer.cpp
    src/gl/Mesh.hpp
    src/gl/Mesh.cpp
    src/gl/Pixmap.hpp
    src/gl/Pixmap.cpp
    src/gl/RenderState.hpp
    src/gl/RenderState.cpp
    src/gl/ShaderProgram.hpp
    src/gl/ShaderProgram.cpp
    src/gl/ShaderUniform.hpp
    src/gl/ShaderUniform.cpp
    src/gl/Texture.hpp
    src/gl/Texture.cpp
    src/gl/VertexLayout.hpp
    src/gl/VertexLayout.cpp
    src/io/File.hpp
    src/io/File.cpp
    src/io/Log.hpp
    src/io/Log.cpp
    src/io/Url.hpp
    src/io/Url.cpp
    src/io/UrlSession.hpp
    src/io/UrlSession.cpp
    src/transform/Transform.hpp
    src/transform/Transform.cpp
    src/view/Camera.hpp
    src/view/Camera.cpp
    src/ImGuiImpl.hpp
    src/ImGuiImpl.cpp
)

add_library(stock ${STOCK_SOURCES})

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${STOCK_SOURCES})

set_target_properties(stock PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(stock PUBLIC
    src
    deps/glm
    deps/stb
)

target_link_libraries(stock PUBLIC
    glad
    glfw
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    imgui
)

add_executable(app
    src/main.cpp
)

set_target_properties(app PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(app PRIVATE
    stock
)

add_subdirectory(tests)

if(WIN32)
    execute_process(COMMAND conan install ${PROJECT_SOURCE_DIR}/conanfile.txt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE CONAN_INSTALL_RESULT
    )
    message(STATUS "Conan install result: ${CONAN_INSTALL_RESULT}")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
    target_link_libraries(stock PUBLIC CONAN_PKG::libcurl)
else()
    target_link_libraries(stock PUBLIC curl)
endif()
