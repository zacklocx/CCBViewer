# - Try to find sqlite3
# Once done this will define
#  SQLITE3_FOUND - System has sqlite3
#  SQLITE3_INCLUDE_DIRS - The sqlite3 include directories
#  SQLITE3_LIBRARIES - The libraries needed to use sqlite3
#  SQLITE3_DEFINITIONS - Compiler switches required for using sqlite3

find_path(SQLITE3_INCLUDE_DIR sqlite3.h
          HINTS /usr/local/opt/sqlite/include)

set(FIND_LIBRARY_SUFFIXES_BACKUP ${CMAKE_FIND_LIBRARY_SUFFIXES})

if(SQLITE3_USE_STATIC_LIBS)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

find_library(SQLITE3_LIBRARY NAMES sqlite3 libsqlite3
             HINTS /usr/local/opt/sqlite/lib)

set(CMAKE_FIND_LIBRARY_SUFFIXES ${FIND_LIBRARY_SUFFIXES_BACKUP})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SQLITE3_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(sqlite3 DEFAULT_MSG
                                  SQLITE3_LIBRARY SQLITE3_INCLUDE_DIR)

mark_as_advanced(SQLITE3_INCLUDE_DIR SQLITE3_LIBRARY)

set(SQLITE3_LIBRARIES ${SQLITE3_LIBRARY})
set(SQLITE3_INCLUDE_DIRS ${SQLITE3_INCLUDE_DIR})