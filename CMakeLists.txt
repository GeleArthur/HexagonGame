cmake_minimum_required(VERSION 3.5)

project(LandvsSea)

file(GLOB helloworld_SRC
     "src/*.h"
     "src/*.cpp"
)

add_executable(LandvsSea
src/Game.cpp
src/Game.h
src/Hexagon.cpp
src/Hexagon.h
src/HexagonManager.cpp
src/HexagonManager.h
src/Layout.cpp
src/Layout.h
src/Orientation.cpp
src/Orientation.h
src/GameEngine/main.cpp 
src/GameEngine/GameEngine.cpp 
src/GameEngine/GameEngine.h 
src/GameEngine/AbstractGame.cpp 
src/GameEngine/AbstractGame.h
src/Utils/Vector2d.cpp
src/Utils/Vector2d.h
src/Utils/Matrix.h
src/Utils/Matrix.cpp
src/Utils/Mouse.h
src/Utils/Texture.h
src/Utils/Texture.cpp
src/Utils/Rect.h
src/Utils/Rect.cpp
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

target_link_libraries(LandvsSea PRIVATE SDL2::SDL2main)
target_link_libraries(LandvsSea PRIVATE SDL2::SDL2 SDL2::SDL2)
target_link_libraries(LandvsSea PRIVATE SDL2_image::SDL2_image)
target_link_libraries(LandvsSea PRIVATE SDL2_ttf::SDL2_ttf)
target_link_libraries(LandvsSea PRIVATE SDL2_mixer::SDL2_mixer)

