# Not needed since there is not vulkan support yet
#find_package(Vulkan REQUIRED)
#
#if(NOT ${Vulkan_FOUND})
#    message(FATAL_ERROR "VulkanSDK must be installed")
#else()
#    message(STATUS "Vulkan Found")
#endif()

file(GLOB_RECURSE roxenginesrc
     "src/*.h"
     "src/*.cpp"
)

option(ROXENGINE_INCLUDE_OPENGL "Include or not opengl" ON)
option(ROXENGINE_INCLUDE_GLFW "Include or not glfw" ON)

add_library(roxengine STATIC ${roxenginesrc})
target_include_directories(roxengine PUBLIC src ${SPDLOG_INCLUDEPATH})
if(${ROXENGINE_INCLUDE_OPENGL})
     target_compile_definitions(roxengine PUBLIC ROXENGINE_INCLUDE_OPENGL)
     target_link_libraries(roxengine PUBLIC glad)
endif()
if(${ROXENGINE_INCLUDE_GLFW})
     target_compile_definitions(roxengine PUBLIC ROXENGINE_INCLUDE_GLFW)
     target_link_libraries(roxengine PUBLIC glfw)
endif()

target_link_libraries(roxengine PUBLIC spdlog)