cmake_minimum_required(VERSION 3.15)

project(StudentRecordSystem
    VERSION 1.0.0
    DESCRIPTION "A trie-backed student record management system"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Release build if none specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# --- Executable ---
add_executable(student_system
    src/main.cpp
    src/student.cpp
    src/system.cpp
)

target_include_directories(student_system
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# --- Install rules  ---
install(TARGETS student_system RUNTIME DESTINATION bin)
