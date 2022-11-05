# Skeleton from : https://github.com/ttroy50/cmake-examples/blob/master/01-basic/B-hello-headers/CMakeLists.txt
# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.2)

# Set the project name
project (solitare)

add_compile_options(-g)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/main.c
    src/game_board/game_board.c
    src/cards/cards.c
    src/utils/utils.c
)

# Add an executable with the above sources
add_executable(solitare ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(solitare
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
