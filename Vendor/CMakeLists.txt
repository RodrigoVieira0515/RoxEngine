macro (install)
endmacro ()

add_subdirectory(glad)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
add_subdirectory(glfw)
option(FLECS_SHARED "Build shared flecs lib" OFF)
add_subdirectory(flecs)
set(SLANG_ENABLE_GFX	OFF)
set(SLANG_ENABLE_SLANGD OFF)
set(SLANG_ENABLE_SLANGC OFF)
set(SLANG_ENABLE_SLANGRT	OFF)
set(SLANG_ENABLE_SLANG_GLSLANG OFF)
set(SLANG_ENABLE_TESTS FALSE)
set(SLANG_LIB_TYPE STATIC)
add_subdirectory(slang)
add_subdirectory(glm)
macro (install)
  _install(${ARGV})
endmacro(install)