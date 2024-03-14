option(SPDLOG_FMT_EXTERNAL_HO "Use external fmt header-only library instead of bundled" ON)
add_subdirectory(spdlog)
add_subdirectory(glad)


option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
add_subdirectory(glfw)

option(FASTFLOAT_TEST "Enable tests" OFF)
option(FASTFLOAT_SANITIZE "Sanitize addresses" OFF)
option(FASTFLOAT_INSTALL "Enable install" ON)
add_subdirectory(fast_float)

option(FMT_PEDANTIC "Enable extra warnings and expensive tests." OFF)
option(FMT_WERROR "Halt the compilation with an error on compiler warnings."
       OFF)
option(FMT_INSTALL "Generate the install target." OFF)
option(FMT_FUZZ "Generate the fuzz target." OFF)
option(FMT_CUDA_TEST "Generate the cuda-test target." OFF)
option(FMT_OS "Include core requiring OS (Windows/Posix) " OFF)
option(FMT_MODULE "Build a module instead of a traditional library." OFF)
option(FMT_SYSTEM_HEADERS "Expose headers with marking them as system." OFF)
add_subdirectory(fmt)

add_subdirectory(frozen)

add_library(nzutils INTERFACE)
target_include_directories(nzutils INTERFACE NazaraUtils/include)

add_library(ordered_map INTERFACE) # Here to avoid installation of ordered_map
target_include_directories(ordered_map INTERFACE ordered_map/include)

file(GLOB_RECURSE nzslsrc
    "nzsl/include/*.h"
    "nzsl/include/*.inl"
    "nzsl/src/NZSL/*.cpp"
)

add_library(nzsl ${nzslsrc})
target_include_directories(nzsl PUBLIC nzsl/include nzsl/src)
target_link_libraries(nzsl PUBLIC nzutils fmt::fmt-header-only frozen ordered_map fast_float)