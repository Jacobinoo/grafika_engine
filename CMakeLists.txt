cmake_minimum_required(VERSION 3.28)
project(GameEngine LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
fetchcontent_declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
fetchcontent_makeavailable(SFML)

add_executable(main
    src/main.cpp
    src/Engine/Engine.cpp
    src/Engine/Input/Input.cpp
    src/Engine/Components/BaseComponent.h
    src/Engine/Components/ShapeComponent.h
    src/Engine/Math/Point2D.cpp
    src/Engine/Math/Primitives.h
    src/Engine/Renderer/Renderer.cpp
    src/Engine/GameObject.cpp
    
    src/DemoMyGame/MyGame.h
    src/DemoMyGame/Ship.h
    src/DemoMyGame/Ufo.h
)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)

# Dokumentacja Doxygen: cmake --build build --target docs
option(BUILD_DOCS "Build API documentation with Doxygen" ON)
if(BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    else()
        message(STATUS "Doxygen not found: 'docs' target will not be available.")
    endif()
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)