# Install script for directory: F:/evo/github/tr-cg/graphics/external/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/Debug/assimp-vc142-mtd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/Release/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/MinSizeRel/assimp-vc142-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/RelWithDebInfo/assimp-vc142-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/Debug/assimp-vc142-mtd.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/Release/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/MinSizeRel/assimp-vc142-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/RelWithDebInfo/assimp-vc142-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/anim.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/aabb.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/ai_assert.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/camera.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/color4.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/color4.inl"
    "F:/evo/github/tr-cg/graphics/external/assimp/build/code/../include/assimp/config.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/defs.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Defines.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/cfileio.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/light.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/material.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/material.inl"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/matrix3x3.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/matrix3x3.inl"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/matrix4x4.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/matrix4x4.inl"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/mesh.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/pbrmaterial.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/postprocess.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/quaternion.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/quaternion.inl"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/scene.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/metadata.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/texture.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/types.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/vector2.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/vector2.inl"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/vector3.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/vector3.inl"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/version.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/cimport.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/importerdesc.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Importer.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/DefaultLogger.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/ProgressHandler.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/IOStream.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/IOSystem.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Logger.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/LogStream.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/NullLogger.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/cexport.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Exporter.hpp"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/DefaultIOStream.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/DefaultIOSystem.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/SceneCombiner.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/fast_atof.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/qnan.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/BaseImporter.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Hash.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/ParsingUtils.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/StreamReader.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/StreamWriter.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/StringComparison.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/StringUtils.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/SGSpatialSort.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/GenericProperty.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/SpatialSort.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/SmoothingGroups.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/SmoothingGroups.inl"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/StandardShapes.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/RemoveComments.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Subdivision.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Vertex.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/LineSplitter.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/TinyFormatter.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Profiler.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/LogAux.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Bitmap.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/XMLTools.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/IOStreamBuffer.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/CreateAnimMesh.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/irrXMLWrapper.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/BlobIOSystem.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/MathFunctions.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Macros.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Exceptional.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Compiler/poppack1.h"
    "F:/evo/github/tr-cg/graphics/external/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/Debug/assimp-vc142-mtd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "F:/evo/github/tr-cg/graphics/external/assimp/build/code/RelWithDebInfo/assimp-vc142-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

