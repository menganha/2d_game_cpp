
add_library(engine STATIC
        src/AssetManager.cpp
        src/Log.cpp
        src/IScene.cpp
        src/Gamepad.cpp
        src/Music.cpp
        src/GameLoop.cpp
        src/Font.cpp
        src/Video.cpp
        src/Texture.cpp
        src/Window.cpp
        src/IMGUI.cpp
    )

# SDL
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2)
find_package(SDL2_image REQUIRED CONFIG COMPONENTS SDL2_image)
find_package(SDL2_mixer REQUIRED CONFIG COMPONENTS SDL2_mixer)
find_package(SDL2_ttf REQUIRED CONFIG COMPONENTS SDL2_ttf)

# FFMPEG -> We can perhaps this on a searcher
find_library(AVCODEC_LIBRARY avcodec)
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVFORMAT_LIBRARY avformat)
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVUTIL_LIBRARY avutil)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(SWSCALE_LIBRARY swscale)
find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)

# Includes and linking
target_include_directories(engine 
    PUBLIC ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/engine
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(engine 
    PUBLIC SDL2::SDL2 SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${SWSCALE_LIBRARY}
)

