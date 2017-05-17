file(GLOB src "src/*.cc")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")
endif()

add_executable(simple ${src})
target_link_libraries(simple vrtk-static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
