INCLUDE_DIRECTORIES(
${PROJECT_SOURCE_DIR}/src
${vki_src}
${vki_src}/base
${vki_src}/vdm
${vki_src}/vis
)

set(InputGen_SRC 
main.cpp
abaqusInputGen.cpp
solverFactory.cpp
solverGen.cpp
)
add_executable(inputGen ${InputGen_SRC})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(inputGen ${vki_lib}/devtools.a)
target_link_libraries(inputGen vkiReader)

install(TARGETS inputGen
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        PUBLIC_HEADER DESTINATION include
        )
