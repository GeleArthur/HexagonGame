cmake_minimum_required(VERSION 3.21)
project(LandvsSea)

include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")


file(GLOB helloworld_SRC
     "src/*.h"
     "src/*.cpp"
)

add_executable(LandvsSea
src/Game.cpp
src/Game.h
src/CameraSystem.cpp
src/CameraSystem.h
src/HexagonManager.cpp
src/HexagonManager.h
src/Piece.cpp
src/Piece.h
src/UISystem.cpp
src/UISystem.h

src/Hexagons/Hexagon.cpp
src/Hexagons/Hexagon.h
src/Hexagons/Cube.cpp
src/Hexagons/Cube.h
src/Hexagons/Layout.cpp
src/Hexagons/Layout.h
src/Hexagons/Orientation.cpp
src/Hexagons/Orientation.h

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
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

target_link_libraries(LandvsSea PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
target_link_libraries(LandvsSea PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(LandvsSea PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)
target_link_libraries(LandvsSea PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
target_link_libraries(LandvsSea PRIVATE ${OPENGL_LIBRARIES})

add_custom_command(TARGET LandvsSea POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/Resources 
                       $<TARGET_FILE_DIR:LandvsSea>
                   COMMENT "Copying resources to build directory"
                   )