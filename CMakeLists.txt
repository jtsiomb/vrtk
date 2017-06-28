cmake_minimum_required(VERSION 2.8.8)
project(vrtk)

include(GNUInstallDirs)

set(SO_MAJOR 0)
set(SO_MINOR 1)

find_package(OpenGL)

option(build_examples "Build example programs" ON)

file(GLOB src "src/*.cc")
file(GLOB hdr "src/*.h")
file(GLOB pubhdr "include/vrtk/*.h")

add_library(vrtk SHARED ${src} ${def} ${hdr} ${pubhdr})
add_library(vrtk-static STATIC ${src} ${hdr} ${pubhdr})

set_target_properties(vrtk PROPERTIES CXX_STANDARD 11)
set_target_properties(vrtk-static PROPERTIES CXX_STANDARD 11)

set_target_properties(vrtk PROPERTIES VERSION ${SO_MAJOR}.${SO_MINOR})
set_target_properties(vrtk PROPERTIES SOVERSION ${SO_MAJOR})

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4996 /wd4305")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")
endif()

target_include_directories(vrtk PUBLIC include PRIVATE include/vrtk src)
target_include_directories(vrtk-static PUBLIC include PRIVATE include/vrtk src)

include_directories(/usr/local/include)

find_library(gmath_lib NAMES gmath libgmath)

target_link_libraries(vrtk ${gmath_lib} ${OPENGL_LIBRARIES})
target_link_libraries(vrtk-static ${gmath_lib} ${OPENGL_LIBRARIES})

install(TARGETS vrtk
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS vrtk-static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${pubhdr} DESTINATION include/vrtk)

if(build_examples)
	add_subdirectory(examples/simple)
endif()
