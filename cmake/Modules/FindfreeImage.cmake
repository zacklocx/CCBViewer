# - Try to find freeimage
# Once done this will define
#  FREEIMAGE_FOUND - System has freeimage
#  FREEIMAGE_INCLUDE_DIRS - The freeimage include directories
#  FREEIMAGE_LIBRARIES - The libraries needed to use freeimage
#  FREEIMAGE_DEFINITIONS - Compiler switches required for using freeimage

find_package(PkgConfig QUIET)
pkg_check_modules(PC_FREEIMAGE QUIET freeimage)
set(FREEIMAGE_DEFINITIONS ${PC_FREEIMAGE_CFLAGS_OTHER})

find_path(FREEIMAGE_INCLUDE_DIR FreeImage.h
          HINTS ${PC_FREEIMAGE_INCLUDEDIR} ${PC_FREEIMAGE_INCLUDE_DIRS})

set(FIND_LIBRARY_SUFFIXES_BACKUP ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

find_library(FREEIMAGE_LIBRARY NAMES freeimage libfreeimage
             HINTS ${PC_FREEIMAGE_LIBDIR} ${PC_FREEIMAGE_LIBRARY_DIRS})

set(CMAKE_FIND_LIBRARY_SUFFIXES ${FIND_LIBRARY_SUFFIXES_BACKUP})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set FREEIMAGE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(freeimage DEFAULT_MSG
                                  FREEIMAGE_LIBRARY FREEIMAGE_INCLUDE_DIR)

mark_as_advanced(FREEIMAGE_INCLUDE_DIR FREEIMAGE_LIBRARY)

set(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})
set(FREEIMAGE_INCLUDE_DIRS ${FREEIMAGE_INCLUDE_DIR})