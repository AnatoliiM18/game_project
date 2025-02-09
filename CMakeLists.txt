#[[
mkdir build
cd build
cmake ..
]]

cmake_minimum_required(VERSION 3.20)

project(
    game_project
    LANGUAGES C
)

set(EXECUTABLE_NAME ${PROJECT_NAME})

add_executable(
    ${EXECUTABLE_NAME}
    src/main.c
)

# Add SDL3
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
    SDL3::SDL3 # If using satelite libraries, SDL must be the last item in the list.
)


if(WIN32)
    # Copies "SDL3.dll", "SDL3_image.dll", "SDL3_ttf.dll" to the directory where executable file is built
    add_custom_command(
        TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_image::SDL3_image> $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_ttf::SDL3_ttf> $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        VERBATIM
    )

    # Set the correct startup project for the Visual Studio
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()
