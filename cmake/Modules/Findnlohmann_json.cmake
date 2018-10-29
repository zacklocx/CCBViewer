# - Try to find nlohmann_json
# Once done this will define
#  NLOHMANN_JSON_FOUND - System has nlohmann_json
#  NLOHMANN_JSON_INCLUDE_DIRS - The nlohmann_json include directories
#  NLOHMANN_JSON_DEFINITIONS - Compiler switches required for using nlohmann_json

find_package(PkgConfig QUIET)
pkg_check_modules(PC_NLOHMANN_JSON QUIET nlohmann_json)
set(NLOHMANN_JSON_DEFINITIONS ${PC_NLOHMANN_JSON_CFLAGS_OTHER})

find_path(NLOHMANN_JSON_INCLUDE_DIR json/json.h
          HINTS ${PC_NLOHMANN_JSON_INCLUDEDIR} ${PC_NLOHMANN_JSON_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set NLOHMANN_JSON_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(nlohmann_json DEFAULT_MSG NLOHMANN_JSON_INCLUDE_DIR)

mark_as_advanced(NLOHMANN_JSON_INCLUDE_DIR)

set(NLOHMANN_JSON_INCLUDE_DIRS ${NLOHMANN_JSON_INCLUDE_DIR})