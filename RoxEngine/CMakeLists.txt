file(GLOB_RECURSE roxenginesrc
     "src/*.h"
     "src/*.cpp"
)

add_library(RoxEngine ${roxenginesrc})
target_include_directories(RoxEngine PUBLIC "src/" "../third_party/projects/glfw/include" "../third_party/projects/shaderc/libshaderc/include" "../third_party/projects" "../third_party/include")
target_link_directories(RoxEngine PUBLIC "../third_party/lib")
target_link_libraries(RoxEngine glfw vulkan-1 glslang shaderc spirv-cross-core spirv-cross-cpp)