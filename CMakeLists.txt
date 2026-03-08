cmake_minimum_required(VERSION 3.5)

project(idk_game CXX)
add_compile_options(-Werror -Wall -Wextra)

add_executable(
    ${PROJECT_NAME}
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h"
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE idk_engine idk_gfx
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game.cpp"
)