cmake_minimum_required(VERSION 2.8)
project(fruitconda)

#find_package(allegro REQUIRED)
set(ALLEGRO_INCLUDE_DIRS "")
set(ALLEGRO_LIBRARIES "-lallegro -lallegro_font -lallegro_image -lallegro_ttf -lallegro_dialog")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)
include_directories(${ALLEGRO_INCLUDE_DIRS})

set(SOURCE_FILES
        src/bitmap.cc
        src/gameplay.cc
        src/input.cc
        src/level.cc
        src/levelselect.cc
        src/list.cc
        src/main.cc
        src/mainmenu.cc
        src/menu.cc
        src/state.cc
        src/util.cc)

add_executable(fruitconda ${SOURCE_FILES})

target_link_libraries(fruitconda ${ALLEGRO_LIBRARIES})
