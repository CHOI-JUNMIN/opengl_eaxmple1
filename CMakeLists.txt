cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME opengl_example1)
set(CMAKE_CXX_STANDARD 17)

set(WINDOW_NAME "Opengl example1")
set(WINDOW_WIDTH 1280)
set(WINDOW_HEIGHT 720)

add_library(implot
    src/implot/implot.cpp
    src/implot/implot_items.cpp
)

target_include_directories(implot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/implot ${CMAKE_CURRENT_SOURCE_DIR}/imgui)


project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/common.cpp src/common.h 
    src/shader.cpp src/shader.h 
    src/program.cpp src/program.h
    src/context.cpp src/context.h
    src/buffer.cpp src/buffer.h
    src/vertex_layout.cpp src/vertex_layout.h
    src/image.cpp src/image.h
    src/texture.cpp src/texture.h
    src/mesh.cpp src/mesh.h
    src/model.cpp src/model.h
    src/context_camera.cpp src/context_grid.cpp
    src/context_render.cpp src/mesh_create.cpp
    src/mesh_render.cpp src/model_load.cpp
    )

include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})


target_link_libraries(${PROJECT_NAME} PUBLIC implot imgui)

target_compile_definitions(${PROJECT_NAME} PUBLIC
WINDOW_NAME="${WINDOW_NAME}"
WINDOW_WIDTH=${WINDOW_WIDTH}
WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})