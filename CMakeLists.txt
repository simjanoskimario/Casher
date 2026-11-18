cmake_minimum_required(VERSION 3.16)
project(Casher CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Casher
        main.cpp
        Database.cpp
        Database.h
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)

target_link_libraries(Casher PRIVATE Qt6::Widgets)

add_executable(CasherConsole
        Console/main_console.cpp
        Console/Database.cpp
        Console/Database.h
)
