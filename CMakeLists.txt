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

if (DEFINED IDK_RUNTIME_STEAMRT4)
    find_package(Steamworks CONFIG REQUIRED)
    target_link_libraries(
        ${PROJECT_NAME} PRIVATE Steamworks::Steamworks
    )
    install(
        FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/steam_appid.txt"
        DESTINATION "${CMAKE_INSTALL_PREFIX}"
    )
endif()

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
