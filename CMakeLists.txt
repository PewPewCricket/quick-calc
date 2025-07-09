cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(quick-calc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.2 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_AUTOMOC ON) # Automatically run MOC on Q_OBJECT classes
set(CMAKE_AUTOGEN_VERBOSE ON)

set(PROJECT_SOURCES
        src/main.cpp
        src/CalculatorWidget.cpp
        src/CalculatorWidget.h
)

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_FORMS}
        ${PROJECT_RESOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)