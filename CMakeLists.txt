cmake_minimum_required(VERSION 4.2)

project(idk_game CXX)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)


add_executable(gamectl "${CMAKE_CURRENT_SOURCE_DIR}/src/gamectl.cpp")
target_compile_features(gamectl PUBLIC cxx_std_23)
target_include_directories(gamectl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h" "${IDK_ROOT_DIR}/include")
target_link_libraries(gamectl PRIVATE libidk idk_imgui idk_engine SDL3::SDL3-shared SDL3_net::SDL3_net)

add_custom_target(
    idk_game_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${IDK_ASSETS_DIRNAME} ${IDK_OUTPUT_DIR}/${IDK_ASSETS_DIRNAME}
)
add_dependencies(${PROJECT_NAME} idk_game_data)
