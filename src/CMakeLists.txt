project(CCBViewer)

set(APP_NAME ${PROJECT_NAME})

set(APP_VERSION_MAJOR 0)
set(APP_VERSION_MINOR 1)
set(APP_VERSION_BUILD 1)

set(APP_DEBUG 1)

configure_file(
	"${PROJECT_SOURCE_DIR}/app_config.h.in"
	"${PROJECT_SOURCE_DIR}/app_config.h"
)

add_definitions(-DSOCI_USE_BOOST)
add_definitions(-DAVHTTP_ENABLE_ZLIB)
add_definitions(-DAVHTTP_ENABLE_OPENSSL)
add_definitions(-DAVHTTP_DISABLE_THREAD)

find_package(zlib REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)
find_package(openssl REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(sqlite3 REQUIRED)
find_package(soci REQUIRED)
find_package(freeglut REQUIRED)
find_package(Threads REQUIRED)

set(EXTRA_LIBS ${EXTRA_LIBS} ${ZLIB_LIBRARIES})
set(EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
set(EXTRA_LIBS ${EXTRA_LIBS} ${OPENSSL_LIBRARIES})
set(EXTRA_LIBS ${EXTRA_LIBS} ${JSONCPP_LIBRARIES})
set(EXTRA_LIBS ${EXTRA_LIBS} ${SQLITE3_LIBRARIES})
set(EXTRA_LIBS ${EXTRA_LIBS} ${SOCI_LIBRARIES})
set(EXTRA_LIBS ${EXTRA_LIBS} ${FREEGLUT_LIBRARIES})
set(EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})

if(APPLE)
find_package(OpenGL REQUIRED)
set(EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARIES})
endif()

include_directories(${Boost_INCLUDE_DIR})

include_directories($ENV{OPENSSL_ROOT_DIR}/include)
link_directories($ENV{OPENSSL_ROOT_DIR}/lib)

include_directories(${JSONCPP_INCLUDE_DIRS})
include_directories(${SOCI_INCLUDE_DIRS})
include_directories(${FREEGLUT_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/third_party/imgui)
include_directories(${CMAKE_SOURCE_DIR}/third_party/avhttp/include)

file(GLOB IMGUI_CPP ${CMAKE_SOURCE_DIR}/third_party/imgui/*.cpp)
file(GLOB_RECURSE APP_CPP *.cpp)

add_executable(${APP_NAME} ${IMGUI_CPP} ${APP_CPP})
target_link_libraries(${APP_NAME} ${EXTRA_LIBS})