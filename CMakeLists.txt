project(game.moonlight)

cmake_minimum_required(VERSION 2.6)
enable_language(CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

ADD_SUBDIRECTORY(lib/moonlight)


find_package(kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(platform REQUIRED)
find_package(TinyXML REQUIRED)
find_package(CURL REQUIRED)

include_directories(${INCLUDES}
                    ${platform_INCLUDE_DIRS}
                    ${kodiplatform_INCLUDE_DIRS}
                    ${TINYXML_INCLUDE_DIRS}
                    ${CURL_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/lib/moonlight/limelight-common)

list(APPEND DEPLIBS ${kodiplatform_LIBRARIES} 
					${platform_LIBRARIES} 
					${TINYXML_LIBRARIES} 
					${CURL_LIBRARIES} 
					moonlight)

set(MOONLIGHT_SOURCES src/addon.cpp
                      src/MoonlightClient.cpp
                      src/Resolution.cpp)

build_addon(game.moonlight MOONLIGHT DEPLIBS)

include(CPack)