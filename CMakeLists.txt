project(game.moonlight)

cmake_minimum_required(VERSION 2.6)
enable_language(CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(platform REQUIRED)

include_directories(${INCLUDES}
                    ${platform_INCLUDE_DIRS}
                    ${kodiplatform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/src)

list(APPEND DEPLIBS ${kodiplatform_LIBRARIES} ${platform_LIBRARIES})

set(MOONLIGHT_SOURCES src/addon.cpp)

build_addon(game.moonlight MOONLIGHT DEPLIBS)

include(CPack)