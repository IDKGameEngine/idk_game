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
    "${CMAKE_CURRENT_SOURCE_DIR}/include" "${IDK_ROOT_DIR}/include"
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE idkengine
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GameImpl.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
)

install(
    CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/steam_appid.txt\" \"480\")"
)
install(
    TARGETS ${PROJECT_NAME}
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
