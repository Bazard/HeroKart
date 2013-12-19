# Install script for directory: C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/build/code/libassimp.dll.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/build/code/libassimp.dll")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.dll")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "C:/Logiciels/MinGW/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.dll")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/anim.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/ai_assert.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/camera.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/color4.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/color4.inl"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/config.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/defs.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/cfileio.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/light.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/material.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/material.inl"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/matrix3x3.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/matrix3x3.inl"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/matrix4x4.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/matrix4x4.inl"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/mesh.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/postprocess.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/quaternion.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/quaternion.inl"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/scene.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/texture.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/types.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/vector2.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/vector2.inl"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/vector3.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/vector3.inl"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/version.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/cimport.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/importerdesc.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/Importer.hpp"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/DefaultLogger.hpp"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/ProgressHandler.hpp"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/IOStream.hpp"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/IOSystem.hpp"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/Logger.hpp"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/LogStream.hpp"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/NullLogger.hpp"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/cexport.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/Exporter.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/../include/assimp/Compiler/poppack1.h"
    "C:/Cours/Synthese d'image/HeroKart/third-party/assimp/code/pstdint.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libassimp3.0-r1264-dev")

