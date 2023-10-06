file(GLOB_RECURSE sandboxsrc
     "src/*.h"
     "src/*.cpp"
)

add_executable(Sandbox ${sandboxsrc})
target_link_libraries(Sandbox RoxEngine)
target_include_directories(Sandbox PRIVATE "../third_party/include")