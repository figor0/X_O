cmake_minimum_required(VERSION 3.5)

project(XVS0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQURIED)
find_package (Boost COMPONENTS unit_test_framework thread system REQUIRED)

set(HEADERS
    include/x_oAI.h
    include/x_omodel.h
    include/proxy.h
    include/player.h
    include/general.h
    include/registrator.h
)
set(SOURCES
    src/x_oAI.cpp
    src/x_omodel.cpp
    src/proxy.cpp
    src/player.cpp
    src/general.cpp
    src/registrator.cpp
)

include_directories(include)
include_directories ( ${Boost_INCLUDE_DIRS} )

add_executable(XVS0
        main.cpp
        qml.qrc
        ${HEADERS}
        ${SOURCES}
)

target_link_libraries(XVS0
  PRIVATE Qt5::Core Qt5::Quick)

enable_testing()

add_executable(mytest test/x_oTests.cpp ${HEADERS} ${SOURCES})

target_link_libraries(mytest
        Qt5::Core Qt5::Quick ${Boost_LIBRARIES})


add_test(NAME x_otest COMMAND mytest)
