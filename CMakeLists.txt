cmake_minimum_required(VERSION 3.31)

project(${IDK_APP_NAME} VERSION 0.0.1 LANGUAGES CXX)
include(GNUInstallDirs)

add_library(idk_game_simulation STATIC
    src/Content.cpp src/Options.cpp src/GameServer.cpp src/ServerLoop.cpp)
target_compile_features(idk_game_simulation PUBLIC cxx_std_23)
target_compile_options(idk_game_simulation PRIVATE -fno-exceptions)
target_include_directories(idk_game_simulation PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_definitions(idk_game_simulation PRIVATE IDK_SERVER_FILENAME="${IDK_APP_NAME}_server")
target_link_libraries(idk_game_simulation PUBLIC idkengine_network idklevel idkgameplay idkengine_timing)

add_executable(${IDK_APP_NAME}_server src/ServerMain.cpp)
target_compile_options(${IDK_APP_NAME}_server PRIVATE -fno-exceptions)
target_link_libraries(${IDK_APP_NAME}_server PRIVATE idk_game_simulation)

add_executable(
    ${PROJECT_NAME}
)
target_compile_options(${PROJECT_NAME} PRIVATE -fno-exceptions)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include" "${IDK_ROOT_DIR}/include"
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE idkengine idk_game_simulation
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GameImpl.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/LocalServer.cpp"
)
add_dependencies(${PROJECT_NAME} ${IDK_APP_NAME}_server)

install(
    CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/steam_appid.txt\" \"480\")"
)
install(
    TARGETS ${PROJECT_NAME} ${IDK_APP_NAME}_server
    DESTINATION "${CMAKE_INSTALL_PREFIX}"
)

if(BUILD_TESTING)
    add_executable(idk_game_network_tests tests/Networking.cpp)
    target_link_libraries(idk_game_network_tests PRIVATE idk_game_simulation idklevel_build)
    target_compile_options(idk_game_network_tests PRIVATE -fno-exceptions)
    add_test(NAME idk_game_network_tests COMMAND idk_game_network_tests)
    set_tests_properties(idk_game_network_tests PROPERTIES TIMEOUT 30)

    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/content/model/sample")
    configure_file("${IDK_ROOT_DIR}/data/examples/instanced-cubes.glb"
        "${CMAKE_CURRENT_BINARY_DIR}/content/model/sample/instanced-cubes.glb" COPYONLY)
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/network-test.cfg"
        "[timing] {\n fixed_hz=30\n physics_hz=120\n render_hz=60\n vsync=0\n}\n")
    add_test(NAME idk_game_local_demo COMMAND ${PROJECT_NAME} --frames 5
        --config "${CMAKE_CURRENT_BINARY_DIR}/network-test.cfg")
    set_tests_properties(idk_game_local_demo PROPERTIES
        ENVIRONMENT "SDL_VIDEODRIVER=offscreen;SDL_AUDIODRIVER=dummy" TIMEOUT 20)

    add_executable(idk_game_process_tests tests/Processes.cpp src/LocalServer.cpp)
    target_link_libraries(idk_game_process_tests PRIVATE idk_game_simulation)
    target_compile_options(idk_game_process_tests PRIVATE -fno-exceptions)
    add_dependencies(idk_game_process_tests ${PROJECT_NAME} idk_playtest_example)
    add_test(NAME idk_game_process_tests COMMAND idk_game_process_tests $<TARGET_FILE:${PROJECT_NAME}>
        "${CMAKE_CURRENT_BINARY_DIR}/network-test.cfg" "${CMAKE_BINARY_DIR}/code/level/finish.idklvl")
    set_tests_properties(idk_game_process_tests PROPERTIES TIMEOUT 45)
endif()
