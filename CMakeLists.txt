project(game.moonlight)

cmake_minimum_required(VERSION 2.6)
enable_language(CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

ADD_SUBDIRECTORY(lib/moonlight)
ADD_SUBDIRECTORY(lib/curlcpp)

find_package(kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(platform REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${INCLUDES}
                    ${platform_INCLUDE_DIRS}
                    ${kodiplatform_INCLUDE_DIRS}
                    ${OPENSSL_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}
                    ${CURLCPP_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/lib/pugixml/src
                    ${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/lib/moonlight/limelight-common)

list(APPEND DEPLIBS ${kodiplatform_LIBRARIES} 
					${platform_LIBRARIES}
          			${OPENSSL_LIBRARIES}
          			curlcpp
					moonlight)

set(MOONLIGHT_SOURCES src/addon.cpp
                      src/MoonlightClient.cpp
                      src/log/Log.cpp
                      src/log/LogAddon.cpp
                      src/log/LogConsole.cpp
                      src/log/LogSyslog.cpp
                      src/nvstream/CertKeyPair.cpp
                      src/nvstream/NvHTTP.cpp
                      src/nvstream/PairingManager.cpp
                      src/settings/Resolution.cpp
                      src/settings/Preferences.cpp
                      lib/pugixml/src/pugixml.cpp)

build_addon(game.moonlight MOONLIGHT DEPLIBS)

include(CPack)
