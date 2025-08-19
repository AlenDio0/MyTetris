cmake_minimum_required(VERSION 3.28)

project(MyTetris LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SFML::Graphics)

if (WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SFML::Main)
endif()

set(ASSETS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/assets)
add_custom_target(COPY_ASSETS
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
    ${ASSETS_FOLDER} $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets
)
set_target_properties(COPY_ASSETS PROPERTIES FOLDER "CMake")
add_dependencies(${CMAKE_PROJECT_NAME} COPY_ASSETS)

file(COPY ${ASSETS_FOLDER} DESTINATION ${CMAKE_BINARY_DIR})
