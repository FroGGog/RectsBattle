cmake_minimum_required(VERSION 3.16)

project(Assigment2 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraires" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)

set(SFML_BUILD_NETWORK FALSE)
FetchContent_MakeAvailable(SFML)

# add imgui to project
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG 35b1148efb839381b84de9290d9caf0b66ad7d03
)

FetchContent_MakeAvailable(imgui)

# add imgui+sfml to project
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml
  GIT_TAG 82dc2033e51b8323857c3ae1cf1f458b3a933c35
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

FetchContent_MakeAvailable(imgui-sfml)

set(SOURCE_FILES

    "src/main.cpp"

    "src/Vec2.cpp"
    "src/Vec2.h"

    "src/Entity.cpp"
    "src/Entity.h"
    
    "src/Game.cpp"
    "src/Game.h"
    
    "src/EntityManager.cpp"
    "src/EntityManager.h"
    
    "src/Components.h")

add_executable(Assigment2 ${SOURCE_FILES})
target_link_libraries(Assigment2 PRIVATE sfml-graphics)
target_link_libraries(Assigment2 PRIVATE ImGui-SFML::ImGui-SFML)
target_compile_features(Assigment2 PRIVATE cxx_std_17)

# Add config file
set(CONFIG_FILE "${CMAKE_SOURCE_DIR}/src/config.txt")

add_custom_command(TARGET Assigment2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CONFIG_FILE} $<TARGET_FILE_DIR:Assigment2>
    COMMENT "Copying CONFIG_FILE file to output directory"
)

if(WIN32)
    add_custom_command(
        TARGET Assigment2
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Assigment2>
        VERBATIM)
endif()