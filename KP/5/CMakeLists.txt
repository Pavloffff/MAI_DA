cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME slzw)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Dialog/Dialog.cpp
    src/Dialog/Dialog.h
    src/Dialog/ArgParser.cpp
    src/Dialog/ArgParser.h
    src/Dialog/Exception.cpp
    src/Dialog/Exception.h
    src/Arifm/Arifm.cpp
    src/Arifm/Arifm.h
    src/Lzw/Lzw.cpp
    src/Lzw/Lzw.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)