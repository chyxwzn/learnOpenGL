get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(../../Common/
                    ../../Vendor/assimp/include/
                    ../../Vendor/bullet/src/
                    ${CMAKE_BINARY_DIR}/Vendor/glad/include/
                    ../../Vendor/glfw/include/
                    ../../Vendor/glm/
                    ../../Vendor/stb/)

file(GLOB PROJECT_SOURCES ../../Common/*.cpp
                          Source/*.cpp)

file(GLOB PROJECT_SHADERS Shader/*.comp
                          Shader/*.frag
                          Shader/*.geom
                          Shader/*.vert)
file(GLOB PROJECT_RESOURCES Resource/*)

source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Resources" FILES ${PROJECT_RESOURCES})
source_group("Shaders" FILES ${PROJECT_SHADERS})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_SHADERS}
                               ${PROJECT_RESOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw glad
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      BulletDynamics BulletCollision LinearMath)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
