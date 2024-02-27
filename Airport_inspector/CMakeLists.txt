cmake_minimum_required(VERSION 3.5)

project(AirportInspector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent Sql Charts Core)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        dbdata.h
        dbdata.cpp
        dbdata.ui
        statistic.cpp
        statistic.h
        statistic.ui
)

    add_executable(AirportInspector
        ${PROJECT_SOURCES}
    )

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)
target_link_libraries(AirportInspector PRIVATE Qt6::Widgets
                                               Qt6::Concurrent
                                               Qt6::Sql
                                               Qt6::Charts
                                               Qt6::Core
                                               libcrypto-3-x64
                                               libiconv-2
                                               libintl-9
                                               libpq
                                               libssl-3-x64)

