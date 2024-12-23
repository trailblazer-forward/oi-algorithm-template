cmake_minimum_required(VERSION 3.5)
project(OI_template)
# find all the source files
file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
FOREACH(SRC ${SRC_LIST})
    get_filename_component(SRC_NAME ${SRC} NAME_WE)
    add_executable(${SRC_NAME} ${SRC})
    set_property(TARGET ${SRC_NAME} PROPERTY CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
ENDFOREACH(SRC)