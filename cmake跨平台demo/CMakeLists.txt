project(InputGen)
SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)
add_subdirectory(reader)
add_subdirectory(src)

