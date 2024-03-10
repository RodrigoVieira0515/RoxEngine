file(GLOB_RECURSE sandboxsrc
     "src/*.h"
     "src/*.cpp"
)

add_executable(sandbox ${sandboxsrc})
target_include_directories(sandbox PRIVATE src)
target_link_libraries(sandbox roxengine)