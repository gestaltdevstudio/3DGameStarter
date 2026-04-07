# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
project(Starter)

set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/..)
set(GPG_SDK_PATH ${CMAKE_SOURCE_DIR}/../Google/gpg-cpp-sdk/android)

add_library(gpg_sdk STATIC IMPORTED)
set_target_properties(gpg_sdk PROPERTIES IMPORTED_LOCATION
    ${GPG_SDK_PATH}/lib/c++/${ANDROID_ABI}/libgpg.a)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -D__ANDROID__ -Wall -Werror -Wno-register -Wno-deprecated-register -Wno-overloaded-virtual -Wno-shift-op-parentheses")

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
   "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(libs_DIR ${CMAKE_SOURCE_DIR}/libs)

add_library(libzip SHARED IMPORTED)
set_target_properties(libzip PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libzip.so)

add_library(libbullet SHARED IMPORTED)
set_target_properties(libbullet PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libbullet.so)

add_library(libopenal SHARED IMPORTED)
set_target_properties(libopenal PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libopenal.so)

add_library(libogg SHARED IMPORTED)
set_target_properties(libogg PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libogg.so)

add_library(libvorbis SHARED IMPORTED)
set_target_properties(libvorbis PROPERTIES IMPORTED_LOCATION
${libs_DIR}/${ANDROID_ABI}/libvorbis.so)

add_library(Starter SHARED
    ${SOURCE_PATH}/android_main.cpp
    ${SOURCE_PATH}/source/android/OS_ANDROID.cpp
    ${SOURCE_PATH}/Google/StateManager.cpp
    # Engine core
    ${SOURCE_PATH}/source/GGEGame.cpp
    ${SOURCE_PATH}/source/Graphics.cpp
    ${SOURCE_PATH}/source/GraphicsManager.cpp
    ${SOURCE_PATH}/source/GraphicsUtils.cpp
    ${SOURCE_PATH}/source/InputSystem.cpp
    ${SOURCE_PATH}/source/Log.cpp
    ${SOURCE_PATH}/source/Resources.cpp
    ${SOURCE_PATH}/source/Shader.cpp
    # 2D
    ${SOURCE_PATH}/source/Animation.cpp
    ${SOURCE_PATH}/source/Button.cpp
    ${SOURCE_PATH}/source/Camera2D.cpp
    ${SOURCE_PATH}/source/Drawable2D.cpp
    ${SOURCE_PATH}/source/Font.cpp
    ${SOURCE_PATH}/source/Render2DPass.cpp
    ${SOURCE_PATH}/source/Render2DPipeline.cpp
    ${SOURCE_PATH}/source/RendererBatch2D.cpp
    ${SOURCE_PATH}/source/Sprite.cpp
    ${SOURCE_PATH}/source/Text.cpp
    ${SOURCE_PATH}/source/TextRenderer.cpp
    ${SOURCE_PATH}/source/TextureAtlas.cpp
    ${SOURCE_PATH}/source/TextureRenderTarget.cpp
    ${SOURCE_PATH}/source/UIObject.cpp
    # 3D
    ${SOURCE_PATH}/source/Camera3D.cpp
    ${SOURCE_PATH}/source/Drawable3D.cpp
    ${SOURCE_PATH}/source/Mesh3D.cpp
    ${SOURCE_PATH}/source/Render3DPass.cpp
    ${SOURCE_PATH}/source/Render3DPipeline.cpp
    ${SOURCE_PATH}/source/Renderer3D.cpp
    # Game
    ${SOURCE_PATH}/source/GameModel.cpp
    ${SOURCE_PATH}/source/GameScreen.cpp
    ${SOURCE_PATH}/source/GameView.cpp
    # GraphicsObjectGL.cpp e OS_GLFW.cpp excluídos: alias legado e desktop-only
)

target_include_directories(Starter PRIVATE
    ${SOURCE_PATH}/conf
    ${ANDROID_NDK}/sources/android/native_app_glue)

include_directories(
        ${SOURCE_PATH}/include/external/libzip
        ${SOURCE_PATH}/include/external
        ${SOURCE_PATH}/Google
        ${GPG_SDK_PATH}/include
        )


# add lib dependencies
target_link_libraries(Starter
    android
    gpg_sdk
    native_app_glue
    EGL
    GLESv3
#    GLESv2
    GLESv1_CM
    log
    libzip
    libopenal
    libogg
    libvorbis
)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
