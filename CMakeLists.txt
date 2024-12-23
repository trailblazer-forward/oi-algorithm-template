cmake_minimum_required(VERSION 3.5)
project(OI_template)
# find all the source files
file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
FOREACH(SRC ${SRC_LIST})
    get_filename_component(SRC_NAME ${SRC} NAME_WE)
    add_executable(${SRC_NAME} ${SRC})
ENDFOREACH(SRC)