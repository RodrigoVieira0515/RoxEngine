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

add_library(roxengine STATIC ${roxenginesrc})
target_include_directories(roxengine PUBLIC src ${SPDLOG_INCLUDEPATH})
target_link_libraries(roxengine PUBLIC spdlog)