cmake_minimum_required(VERSION 3.16)
project(CodeEditor VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
set(SOURCES
    src/main.cpp
    src/Editor.cpp
    src/CodeEditor.cpp
    src/SyntaxHighlighter.cpp
    src/FindReplaceDialog.cpp
    src/ThemeManager.cpp
)
add_executable(CodeEditor ${SOURCES})
target_link_libraries(CodeEditor PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
