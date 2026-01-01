cmake_minimum_required(VERSION 3.28)
project(status_line C)

set(CMAKE_C_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGIT2 REQUIRED libgit2)

add_executable(status-line src/main.c src/git_functions.c src/text_formatting.c)

execute_process(
    COMMAND pkg-config --libs --static libgit2
    OUTPUT_VARIABLE LIBGIT2_STATIC_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REGEX REPLACE "-l" "" LIBGIT2_LIBS_CLEAN "${LIBGIT2_STATIC_LIBS}")
string(REPLACE " " ";" LIBGIT2_LIBS_LIST "${LIBGIT2_LIBS_CLEAN}")

target_link_libraries(status-line ${LIBGIT2_LIBS_LIST})
target_include_directories(status-line PRIVATE ${LIBGIT2_INCLUDE_DIRS})