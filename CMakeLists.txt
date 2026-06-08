cmake_minimum_required(VERSION 3.16...4.3)   # min 3.16, max 4.3
project(Starter)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "Enable X11" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

file(COPY ${CPP_DIR}/conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
                
IF(APPLE)

   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(OPENGL_LIBRARY OpenGL )
   FIND_LIBRARY(IOKIT_LIBRARY IOKit )
   FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo )
   FIND_LIBRARY(OPENAL_LIBRARY OpenAL )

   file(GLOB RSC_FILES "${CMAKE_BINARY_DIR}/conf/*")

ENDIF (APPLE)

 
if (MSVC AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "Windows build must be x64. Configure with: cmake -S . -B build_x64 -A x64")
endif ()

include_directories(${CPP_DIR}/include ${CPP_DIR}/include/external/libzip ${CPP_DIR}/include/external)
file(GLOB SOURCES "${CPP_DIR}/main.cpp" "${CPP_DIR}/source/*.cpp" "${CPP_DIR}/source/external/*.c")
# GraphicsObjectGL.cpp é um alias legado de Drawable3D.cpp; excluir do build CMake.
list(REMOVE_ITEM SOURCES "${CPP_DIR}/source/GraphicsObjectGL.cpp")

if (UNIX AND NOT APPLE)
  set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland support" FORCE)
  set(GLFW_BUILD_X11 ON CACHE BOOL "Enable X11 support" FORCE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -DGGE_LINUX")
endif ()
 
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGGE_DESKTOP")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -DGGE_DESKTOP")
endif ()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

ADD_LIBRARY(glfw STATIC IMPORTED)       
if (APPLE)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libglfw3.a)   
elseif (UNIX)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libglfw3.a)   
endif ()
   
if (MSVC)  
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/glfw3.lib)   
endif (MSVC)

ADD_LIBRARY(libzip STATIC IMPORTED)   
if (APPLE)
  SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libzip.a) 
elseif (UNIX)
  SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libzip.a) 
endif ()        
   
if (MSVC)
  SET_TARGET_PROPERTIES(libzip PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/zip.lib) 
endif (MSVC)

if (APPLE)               
  ADD_LIBRARY(libvorbisfile STATIC IMPORTED)   
  SET_TARGET_PROPERTIES(libvorbisfile PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libvorbisfile.a)    
elseif (UNIX)               
  ADD_LIBRARY(libvorbisfile STATIC IMPORTED)   
  SET_TARGET_PROPERTIES(libvorbisfile PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libvorbisfile.a)    
endif ()

if (MSVC)
  ADD_LIBRARY(libvorbisfile STATIC IMPORTED)
  SET_TARGET_PROPERTIES(libvorbisfile PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/vorbisfile.lib)
endif (MSVC)

if (MSVC)              
  ADD_LIBRARY(zlib STATIC IMPORTED)   
  SET_TARGET_PROPERTIES(zlib PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/zsd.lib)    
endif (MSVC)


ADD_LIBRARY(libvorbis STATIC IMPORTED) 
if (APPLE)
  SET_TARGET_PROPERTIES(libvorbis PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libvorbis.a) 
elseif (UNIX)
  SET_TARGET_PROPERTIES(libvorbis PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libvorbis.a) 
endif ()

if (MSVC)
  SET_TARGET_PROPERTIES(libvorbis PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/vorbis.lib) 
endif (MSVC)

ADD_LIBRARY(libopenal STATIC IMPORTED)   
if (UNIX)
  SET_TARGET_PROPERTIES(libopenal PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libopenal.a)    
endif (UNIX)

if (MSVC)
  SET_TARGET_PROPERTIES(libopenal PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/OpenAL32.lib)    
endif (MSVC)


ADD_LIBRARY(libogg STATIC IMPORTED)   
if (APPLE)
  SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsOSX/libogg.a) 
elseif (UNIX)
  SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsLinux/debug/libogg.a) 
endif ()

if (MSVC)
SET_TARGET_PROPERTIES(libogg PROPERTIES IMPORTED_LOCATION ${CPP_DIR}/libsWin/debug/ogg.lib) 
endif (MSVC)              

if (APPLE)
add_executable(Starter MACOSX_BUNDLE ${SOURCES} ${RSC_FILES})
else()
add_executable(Starter ${SOURCES})
endif()

# Keep runtime assets and Windows DLLs beside the executable for Debug/Release runs.
add_custom_command(TARGET Starter POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CPP_DIR}/conf
          $<TARGET_FILE_DIR:Starter>/conf)

if (MSVC)
  add_custom_command(TARGET Starter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CPP_DIR}/libsWin/ogg.dll
            ${CPP_DIR}/libsWin/vorbis.dll
            ${CPP_DIR}/libsWin/vorbisfile.dll
            ${CPP_DIR}/libsWin/zip.dll
            ${CPP_DIR}/libsWin/OpenAL32.dll
            $<TARGET_FILE_DIR:Starter>)
endif (MSVC)

if (APPLE)
  target_link_libraries (Starter glfw ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${OPENAL_LIBRARY} z bz2 libzip libvorbisfile libvorbis libogg)  

set_source_files_properties(${RSC_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/conf)

elseif (UNIX)
  target_link_libraries (Starter PRIVATE libzip libvorbisfile libvorbis libopenal libogg PUBLIC glfw GL X11 Xxf86vm Xinerama Xcursor Xrandr dl z)  
endif ()                  

if (MSVC)
  target_compile_definitions(Starter PRIVATE ZIP_STATIC)
  target_link_libraries (Starter glu32.lib opengl32.lib libzip libvorbisfile libvorbis libopenal libogg zlib glfw )  
endif (MSVC)
