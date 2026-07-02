cmake_minimum_required(VERSION 4.2)

project(idk_game CXX)
add_compile_options(-Werror -Wall -Wextra)

add_executable(
    ${PROJECT_NAME}
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/h" "${IDK_ROOT_DIR}/include"
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE idk_engine idk_gfx
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_service.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)


# add_executable(udp_client src/udp_client.cpp)
# target_compile_features(udp_client PUBLIC cxx_std_23)
# target_include_directories(udp_client PUBLIC "${IDK_ROOT_DIR}/include")

# add_executable(udp_server src/udp_server.cpp)
# target_compile_features(udp_server PUBLIC cxx_std_23)
# target_include_directories(udp_server PUBLIC "${IDK_ROOT_DIR}/include")

add_custom_target(
    idk_game_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/asset ${IDK_OUTPUT_DIR}/asset
)
add_dependencies(${PROJECT_NAME} idk_game_assets)
