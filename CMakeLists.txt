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
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GameService.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)


add_executable(mkpkg "${CMAKE_CURRENT_SOURCE_DIR}/src/mkpkg.cpp")
target_compile_features(mkpkg PUBLIC cxx_std_23)
target_include_directories(mkpkg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h" "${IDK_ROOT_DIR}/include")
target_link_libraries(mkpkg PRIVATE idk_core)

add_executable(unpkg "${CMAKE_CURRENT_SOURCE_DIR}/src/unpkg.cpp")
target_compile_features(unpkg PUBLIC cxx_std_23)
target_include_directories(unpkg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h" "${IDK_ROOT_DIR}/include")
target_link_libraries(unpkg PRIVATE idk_core)



# add_executable(udp_client src/udp_client.cpp)
# target_compile_features(udp_client PUBLIC cxx_std_23)
# target_include_directories(udp_client PUBLIC "${IDK_ROOT_DIR}/include")

# add_executable(udp_server src/udp_server.cpp)
# target_compile_features(udp_server PUBLIC cxx_std_23)
# target_include_directories(udp_server PUBLIC "${IDK_ROOT_DIR}/include")

add_custom_target(
    idk_game_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/${IDK_ASSETS_DIRNAME} ${IDK_OUTPUT_DIR}/${IDK_ASSETS_DIRNAME}
)
add_dependencies(${PROJECT_NAME} idk_game_data)
