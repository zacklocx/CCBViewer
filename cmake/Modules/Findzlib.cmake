# - Try to find zlib
# Once done this will define
#  ZLIB_FOUND - System has zlib
#  ZLIB_INCLUDE_DIRS - The zlib include directories
#  ZLIB_LIBRARIES - The libraries needed to use zlib
#  ZLIB_DEFINITIONS - Compiler switches required for using zlib

find_path(ZLIB_INCLUDE_DIR zlib.h
          HINTS /usr/local/opt/zlib/include)

set(FIND_LIBRARY_SUFFIXES_BACKUP ${CMAKE_FIND_LIBRARY_SUFFIXES})

if(ZLIB_USE_STATIC_LIBS)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

find_library(ZLIB_LIBRARY NAMES z libz
             HINTS /usr/local/opt/zlib/lib)

set(CMAKE_FIND_LIBRARY_SUFFIXES ${FIND_LIBRARY_SUFFIXES_BACKUP})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ZLIB_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(zlib DEFAULT_MSG
                                  ZLIB_LIBRARY ZLIB_INCLUDE_DIR)

mark_as_advanced(ZLIB_INCLUDE_DIR ZLIB_LIBRARY)

set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})