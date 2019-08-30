cmake_minimum_required(VERSION 2.8.11)
 
if (POLICY CMP0020)
  cmake_policy (SET CMP0020 NEW)
endif()

if (POLICY CMP0043)
  cmake_policy (SET CMP0043 NEW)
endif()

if (POLICY CMP0071)
  cmake_policy (SET CMP0071 NEW)
endif()
if (POLICY CMP0053)
  cmake_policy (SET CMP0053 NEW)
endif()
if (POLICY CMP0022)
  cmake_policy (SET CMP0022 NEW)
endif()
set(CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake"
  ${CMAKE_MODULE_PATH}
)
SET(Qt5_DIR C:/Qt/Qt5.9.1/5.9.1/msvc2015_64/lib/cmake/Qt5)
find_package(QT5 REQUIRED)
SET(projectName MaterialLib)
PROJECT(${projectName})
set(CMAKE_MODULE_PATH ${MyProject_CMAKE_DIR} ${CMAKE_MODULE_PATH})

add_definitions(-DWNT)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
) 
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Xml Charts)

file(GLOB HeaderFiles ${CMAKE_CURRENT_SOURCE_DIR}/*h)
file(GLOB SrcFiles ${CMAKE_CURRENT_SOURCE_DIR}/*cpp)
file(GLOB UiFiles *.ui )
file(GLOB ResFilses *.qrc )

qt5_wrap_cpp(MOCSrcs ${HeaderFiles})
qt5_wrap_ui(UiSrcs ${UiFiles})
qt5_add_resources(RccScrs ${ResFilses})

add_library(${projectName} SHARED
  ${SrcFiles} 
  ${HeaderFiles}  
  ${MOCSrcs}  
  ${RccScrs}
  ${UI_FILES}
)

SOURCE_GROUP("MaterialHeaders" FILES 
  ${HeaderFiles} 
)

SOURCE_GROUP("MaterialSources" FILES 
  ${SrcFiles} 
)


SOURCE_GROUP("Generated\\MocHeaders" FILES 
	${MOCSrcs}
)

qt5_use_modules(${projectName} Core Gui Widgets OpenGL Xml Charts)



