cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME slzw)
project(${PROJECT_NAME})

add_compile_options(-g)


add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Arifm/Frequency.cpp
    src/Arifm/Frequency.h
    src/Arifm/Arifm.cpp
    src/Arifm/Arifm.h
    src/Lzw/Lzw.cpp
    src/Lzw/Lzw.h
    src/Dialog/Dialog.cpp
    src/Dialog/Dialog.h
    src/Dialog/ArgParser.cpp
    src/Dialog/ArgParser.h
    src/Dialog/Exception.cpp
    src/Dialog/Exception.h
    src/Utils/BitStream.cpp
    src/Utils/BitStream.h
    src/Utils/Utils.cpp
    src/Utils/Utils.h
)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)