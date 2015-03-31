cmake_minimum_required(VERSION 3.2.1)
project(pythonqt-test C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(PythonLibs 3.4 REQUIRED)

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

if(0)
find_package(PythonInterp 3.4 REQUIRED)
get_filename_component(pythonPath ${PYTHON_EXECUTABLE} DIRECTORY)
message(STATUS "Python directory: ${pythonPath}")

# PythonLibs should be found after PythonInterp, per the CMake docs.
find_package(PythonLibs 3.4 REQUIRED)
get_filename_component(pythonLib ${PYTHON_LIBRARIES} DIRECTORY)
message(STATUS "Python library directory: ${pythonLib}")

find_program(qmakeExe qmake)
get_filename_component(qtBinPath ${qmakeExe} DIRECTORY)
file(TO_NATIVE_PATH "${qtBinPath}" qtBinPath)
message(STATUS "Qt binary path: ${qtBinPath}")
set(pathWithQt "$ENV{PATH};${qtBinPath}")
#string(REGEX REPLACE "\\\\" "\\\\\\\\" pathWithQt ${pathWithQt})
message(STATUS "${pathWithQt}")

find_program(nmakeExe nmake)


include(ExternalProject)

# ExternalProject can also use existing source directories, so we could include
# pythonqt as a submodule instead of gitting it separately.
externalproject_add(pythonqt
    GIT_REPOSITORY https://github.com/BaroboRobotics/pythonqt.git
    GIT_TAG develop
    CONFIGURE_COMMAND "${CMAKE_COMMAND}" -E env
    	PYTHON_PATH="${pythonPath}"
		PYTHON_LIB="${pythonLib}"
		"PATH=${pathWithQt}"
		"${qmakeExe}"
	BUILD_COMMAND "${CMAKE_COMMAND}" -E env
		PYTHON_PATH="${pythonPath}"
		PYTHON_LIB="${pythonLib}"
		"PATH=${pathWithQt}"
		"${nmakeExe}"
	BUILD_IN_SOURCE 1		# Avoid manually passing .pro file to qmake
    )
endif()