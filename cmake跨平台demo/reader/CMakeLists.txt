SET (vki_root  "" CACHE PATH "vki_root")

if("${vki_root}" STREQUAL "")
  MESSAGE (FATAL_ERROR "NOT FOUND: Products_Dir" )
else()
	SET (vki_lib  ${vki_root}/lib/LINUX64 CACHE INTERNAL "")
	SET (vki_src  ${vki_root}/src CACHE INTERNAL "")
endif()

INCLUDE_DIRECTORIES(
${vki_src}
${vki_src}/base
${vki_src}/vdm
${vki_src}/vis
)
LINK_DIRECTORIES(${vki_lib})

set(LIB_SRC VKIReader.cpp)
#set(CMAKE_C_FLAGS "-fPIC")
add_library(vkiReader ${LIB_SRC})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
TARGET_LINK_LIBRARIES(vkiReader ${vki_lib}/devtools.a)
