cmake_minimum_required(VERSION 3.16)

project(platformer_2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
 
option(TILE_TEST "Build tile_test only" OFF)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

if(TILE_TEST)
    message(STATUS "Configuring Tile Test build")

    add_executable(tile_test
        tile_test.cpp
    )

    target_include_directories(tile_test
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )

    target_link_libraries(tile_test
        PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        spdlog::spdlog
        nlohmann_json::nlohmann_json
    )

else()
    message(STATUS "Configuring Full Game build")

    file(GLOB_RECURSE PROJECT_SOURCES
        src/*.cpp
    )

    add_executable(${PROJECT_NAME}
        main.cpp
        ${PROJECT_SOURCES}
    )

    target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        spdlog::spdlog
        nlohmann_json::nlohmann_json
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()
