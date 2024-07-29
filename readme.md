# YASTUMP

YASTUMP (Yet Another Shoot'em Up) is a experiment in trying to add a video
player together with a video game.

The game features a small space ship destroying enemies while the music and
videos of *your choice* play in the background.

## Features

* Classic tried-and-true fun shooter gameplay.
* Different types of enemies, including the dreaded **text-type** enemy.
* A flexible [Lua](https://www.lua.org/) API that allows to construct levels
with enemies, songs, and videos of your choice.

## How to build

The project requires the SDL2 and FFmpeg external libraries to be installed in
your system. Additionally, you need Cmake (ver >= 3.29) and a C++ compiler
supporting C++17.

### Linux
If all these requirements are met run the following commands on a terminal
```
git clone --recurse-submodules https://github.com/menganha/2d_game_cpp.git
cd 2d_game_cpp
mkdir build
cmake --build build
cmake -G Ninja -B build -S .
```
this will clone the repo into a local folder together with the [Entt](https://github.com/skypjack/entt)
dependency and build the game. To run it, simply execute the `app` application.

### Windows
Not yet supported

## How to create new levels
In construction

