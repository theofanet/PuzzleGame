cmake_minimum_required(VERSION 3.5)
project(PuzzelGame)

INCLUDE_DIRECTORIES(${PuzzelGame_SOURCE_DIR}/includes)

IF(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -lmingw32")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    set(EXTRA_LIBS -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
ENDIF(WIN32)

IF(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    INCLUDE_DIRECTORIES (/Library/Frameworks)

    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(SDL2_LIB SDL2)
    FIND_LIBRARY(SDL2_IMG_LIB SDL2_image)
    FIND_LIBRARY(SDL2_TTF_LIB SDL2_ttf)

    MARK_AS_ADVANCED (COCOA_LIBRARY)
    MARK_AS_ADVANCED (SDL2_LIB)
    MARK_AS_ADVANCED (SDL2_IMG_LIB)
    MARK_AS_ADVANCED (SDL2_TTF_LIB)

    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${SDL2_LIB} ${SDL2_IMG_LIB} ${SDL2_TTF_LIB})
ENDIF (APPLE)

set(
        SOURCE_FILES main.cpp
        includes/Globals.h
        includes/Utils.h
        includes/LinkedList.h
        includes/Game.h src/Game.cpp
        includes/Input.h src/Input.cpp
        includes/Entities/Player.h src/Entities/Player.cpp
        includes/GGraph/Sprite.h src/GGraph/Sprite.cpp
        includes/GGraph/Tile.h src/GGraph/Tile.cpp
        includes/GGraph/AnimatedSprite.h src/GGraph/AnimatedSprite.cpp
        includes/GGraph/AnimatedTile.h src/GGraph/AnimatedTile.cpp
        includes/GGraph/Graphics.h src/GGraph/Graphics.cpp
        src/GPhy/GObject.cpp includes/GPhy/GObject.h
        includes/GPhy/GPhy.h src/GPhy/GWorld.cpp includes/GPhy/GWorld.h src/GPhy/GRect.cpp includes/GPhy/GRect.h src/GPhy/GCollision.cpp includes/GPhy/GCollision.h
        src/Level.cpp includes/Level.h
        includes/tinyxml2.h src/tinyxml2.cpp src/Entities/Door.cpp includes/Entities/Door.h includes/Entities/Enemy.h src/Entities/Snake.cpp includes/Entities/Snake.h includes/GGraph/ActiveTile.h src/GGraph/ActiveTile.cpp src/GGraph/Font.cpp includes/GGraph/Font.h src/GGraph/Camera.cpp includes/GGraph/Camera.h src/Entities/Stairs.cpp includes/Entities/Stairs.h src/GGraph/HUD.cpp includes/GGraph/HUD.h)

add_executable(PuzzelGame ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(PuzzelGame ${EXTRA_LIBS})

add_custom_command(TARGET PuzzelGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/content)
add_custom_command(TARGET PuzzelGame POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/content ${CMAKE_BINARY_DIR}/content)