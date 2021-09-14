cmake_minimum_required(VERSION 3.0)
project(visu C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -march=native)  # march=native for best performance
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -Wno-unused-function")

include_directories(
		${PROJECT_SOURCE_DIR}/include
)
file(GLOB SRCS
		"${PROJECT_SOURCE_DIR}/src/*"
		"${PROJECT_SOURCE_DIR}/src/camera/*"
		"${PROJECT_SOURCE_DIR}/src/control/*"
		"${PROJECT_SOURCE_DIR}/src/gtk/*"
		"${PROJECT_SOURCE_DIR}/src/object/*"
		)

add_library(visu SHARED ${SRCS})
set_target_properties(visu PROPERTIES LINKER_LANGUAGE C)

set_property(TARGET visu PROPERTY POSITION_INDEPENDENT_CODE ON)

#
# link here
#
find_library(PBASE_LIB pbase required)
target_link_libraries(visu PUBLIC ${PBASE_LIB})
if (MINGW)
	include_directories(C:/Libraries/include)
endif()

# GTK + OpenGL
find_package(PkgConfig QUIET)
pkg_check_modules(GTK gtk+-3.0)
pkg_check_modules(EPOXY epoxy)

target_include_directories(visu PUBLIC ${GTK_INCLUDE_DIRS} ${EPOXY_INCLUDE_DIRS})
target_link_libraries(visu PUBLIC ${GTK_LIBRARIES} ${EPOXY_LIBRARIES})



#
# install:
#

if (MINGW)
	set(INSTALL_LIB "C:/Libraries/lib")
	set(INSTALL_INCLUDE "C:/Libraries/include")
	add_definitions(-DP_RHC_LOG_DO_NOT_USE_COLOR)
	add_definitions(-DMATHC_NO_PRINT_COLOR)
else()
	set(INSTALL_LIB lib)
	set(INSTALL_INCLUDE include)
endif()

install(TARGETS visu
		LIBRARY DESTINATION ${INSTALL_LIB}
		ARCHIVE DESTINATION ${INSTALL_LIB}
		RUNTIME DESTINATION ${INSTALL_LIB}
		)
install(DIRECTORY include/ DESTINATION ${INSTALL_INCLUDE})

# install python
set(PYTHON_LIB_INSTALL_DIR $ENV{PYTHON_LIB_INSTALL_DIR})
if(PYTHON_LIB_INSTALL_DIR)
	set(PYTHON_LIB_INSTALL_DIR ${PYTHON_LIB_INSTALL_DIR} /visu)
	string(REPLACE "\\" "/" PYTHON_LIB_INSTALL_DIR ${PYTHON_LIB_INSTALL_DIR})
 	install(DIRECTORY python/visu/ DESTINATION ${PYTHON_LIB_INSTALL_DIR})
else()
	message(AUTHOR_WARNING "env variable PYTHON_LIB_INSTALL_DIR not set, not installing python modules")
endif()


#
# Testing
#

# Enables ctest (after "make", test with "ctest" or "ctest -V")
#enable_testing()

# Define the executable test
add_executable(main tests/main.c)
add_executable(light_tool tests/light_tool.c)
add_executable(example_visuwidget tests/visuwidget_example.c)
add_executable(example_svisu tests/svisu_example.c)

# link to the library
target_link_libraries(main visu)
target_link_libraries(light_tool visu)
target_link_libraries(example_visuwidget visu)
target_link_libraries(example_svisu visu)


# add the executable as test

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
