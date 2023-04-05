cmake_minimum_required(VERSION 3.0)

project(MyProject)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/virus_total.cpp
)

set(HEADERS
    ${SOURCE_DIR}/virus_total.h
)

find_library(WINHTTP_LIBRARY Winhttp)

add_executable(MyProject ${SOURCES} ${HEADERS})

target_link_libraries(MyProject ${WINHTTP_LIBRARY})

target_compile_features(MyProject PRIVATE cxx_std_11)

if(MSVC)
    target_compile_options(MyProject PRIVATE /W4)
else()
    target_compile_options(MyProject PRIVATE -Wall -Wextra -pedantic)
endif()

target_compile_definitions(MyProject PRIVATE VIRUSTOTAL_API_KEY="your_api_key_here")
Give feedback
