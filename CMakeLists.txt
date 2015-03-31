cmake_minimum_required(VERSION 3.2.1)
project(pythonqt-test C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(NOT DEFINED ENV{PYTHONQT_ROOT})
	message(FATAL_ERROR "PYTHONQT_ROOT is not set")
endif()

find_package(Qt5Core)
find_package(PythonLibs 3.4 REQUIRED) # 3.4 is hard-coded into our fork of pythonqt

add_executable(main main.cpp)

target_link_libraries(main
	PRIVATE Qt5::Core
			$ENV{PYTHONQT_ROOT}/lib/PythonQt.lib
			${PYTHON_LIBRARIES}
			)

target_include_directories(main
	PRIVATE $ENV{PYTHONQT_ROOT}/src
			${PYTHON_INCLUDE_DIRS}
			)