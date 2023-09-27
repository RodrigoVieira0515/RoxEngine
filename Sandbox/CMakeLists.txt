file(GLOB_RECURSE sandboxsrc
     "src/*.h"
     "src/*.cpp"
)

add_executable(Sandbox ${sandboxsrc})
target_link_libraries(Sandbox RoxEngine)
