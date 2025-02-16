# Yastump - (Yet Another Shoot'em Up)

Yastump (Yet Another Shoot'em Up) is a experiment in streaming video on
a game background using ffmpeg.

The game features a small space ship destroying enemies while the music and
videos of *your choice* play in the background.

You can set up the levels via the lua API

<p align="center">
  <img src="./demo.gif"  alt=""/>
</p>

## Features

* Classic tried-and-true fun shooter gameplay.
* Different types of enemies, including the dreaded **text-type** enemy.
* A flexible [Lua](https://www.lua.org/) API that allows to construct levels
with enemies, songs, and videos of your choice.

## How to build

The project requires the SDL2 and FFmpeg external libraries to be installed in
your system. Additionally, you need Cmake (ver >= 3.29) and a C++ compiler
supporting C++17.

## How to create new levels

Start by creating a `level<number>.lua` file in the script folder, where the `<number>` is 
the level number starting from zero. The API is very simple. Use `yage.set_music` and 
`yage.set_video` for pointing the background music and video file path. The video backend is ffmpeg so a variety of
video formats are supported.

Enemies are defined as simple Lua arrays of 4 elements which are added to a level with the 
`yage.add_enemy` function.
* Enemy type. Two types are supported: "seeker" and "simple".
* Spawn position on the x axis
* Spawn position on the y axis
* Number of frames after the level start when they should spawn

A complete example would look like the following

```lua
yage.set_music("music/penso_positivo.wav")
yage.set_video("videos/clouds.mp4")

local enemies = {
    {"seeker", 40, 300, 0}, {"seeker", 40, 400, 0}, {"simple", 65, 20, 0},
    {"simple", 40, 300, 10}, {"simple", 40, 400, 20}, {"simple", 65, 20, 20},
}

for _, ene in ipairs(enemies) do
    yage.add_enemy(ene[1], ene[2], ene[3], ene[4])
end

yage.add_text([[
This is the second level.
Please eliminate all the letters on screen!
]])
```

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


