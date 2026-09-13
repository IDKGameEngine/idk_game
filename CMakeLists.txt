cmake_minimum_required(VERSION 3.31)

project(${IDK_APP_NAME} VERSION 0.0.1 LANGUAGES CXX)
include(GNUInstallDirs)

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
    ${PROJECT_NAME} PRIVATE idkengine idkgfx
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/steam_appid.txt"
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
)

install(TARGETS ${PROJECT_NAME} DESTINATION "./")
file(GLOB SHARED_LIBRARY_FILES "${CMAKE_PREFIX_PATH}/lib/*.so*")
install(FILES ${SHARED_LIBRARY_FILES} DESTINATION "${CMAKE_INSTALL_LIBDIR}/")

# add_custom_target(
#     idk_game_data
#     COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
#         ${CMAKE_CURRENT_SOURCE_DIR}/${IDK_ASSETS_DIRNAME} ${IDK_OUTPUT_DIR}/${IDK_ASSETS_DIRNAME}
# )
# add_dependencies(${PROJECT_NAME} idk_game_data)
