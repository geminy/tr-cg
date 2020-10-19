IF(NOT EXISTS "F:/evo/github/tr-cg/graphics/external/assimp/build/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"F:/evo/github/tr-cg/graphics/external/assimp/build/install_manifest.txt\"")
ENDIF(NOT EXISTS "F:/evo/github/tr-cg/graphics/external/assimp/build/install_manifest.txt")

FILE(READ "F:/evo/github/tr-cg/graphics/external/assimp/build/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "F:/Software/cmake-3.18.3-win64-x64/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
