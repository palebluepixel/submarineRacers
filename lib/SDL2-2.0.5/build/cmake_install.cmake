# Install script for directory: /home/ashwin/repo/submarines/lib/SDL2-2.0.5

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/libSDL2.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/libSDL2-2.0.so.0.4.1"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/libSDL2-2.0.so.1"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/libSDL2-2.0.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2-2.0.so.0.4.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2-2.0.so.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL2-2.0.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/libSDL2main.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_crc32.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/close_code.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_syswm.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_minimal.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_cpuinfo.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_render.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_keycode.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_thread.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/begin_code.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_log.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_stdinc.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_assert.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_quit.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_iphoneos.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_surface.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_messagebox.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_compare.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_opengles2_gl2.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_opengl.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_audio.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_opengles.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_mouse.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_android.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_events.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_error.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_rect.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_gamecontroller.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_assert.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_random.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_clipboard.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_timer.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_version.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_pandora.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_revision.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_opengles2.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_name.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_system.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_wiz.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_types.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_fuzzer.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_winrt.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_keyboard.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_egl.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_mutex.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_scancode.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_gesture.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_bits.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_opengles2_gl2platform.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_loadso.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_rwops.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_harness.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_blendmode.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_atomic.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_platform.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_main.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_common.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_font.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_opengles2_khrplatform.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_windows.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_copying.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_endian.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_filesystem.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_md5.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_pixels.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_macosx.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_joystick.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_haptic.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_config_psp.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_shape.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_power.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_touch.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_hints.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_images.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_video.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_test_log.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_opengles2_gl2ext.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/include/SDL_opengl_glext.h"
    "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/include/SDL_config.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  
      execute_process(COMMAND /usr/bin/cmake -E create_symlink
      "libSDL2-2.0.so" "libSDL2.so")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/libSDL2.so")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/sdl2.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/sdl2-config")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/share/aclocal/sdl2.m4")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/share/aclocal" TYPE FILE FILES "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/sdl2.m4")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ashwin/repo/submarines/lib/SDL2-2.0.5/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
