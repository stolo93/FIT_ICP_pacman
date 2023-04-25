cmake_minimum_required(VERSION 3.5)

project(pacman VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        GUI/main_window.cpp GUI/main_window.h
        GUI/ScreenMenu/widget_main_menu.cpp GUI/ScreenMenu/widget_main_menu.h
        GUI/ScreenMenu/screen_menu.cpp GUI/ScreenMenu/screen_menu.h
        GUI/ScreenNewGame/screen_new_game.cpp
        GUI/ScreenNewGame/widget_text_input_dialog.cpp
        GUI/ScreenNewGame/widget_map_file_dialog.h GUI/ScreenNewGame/widget_map_file_dialog.cpp
        GUI/ScreenLoadGame/widget_file_input_dialog.h GUI/ScreenLoadGame/widget_file_input_dialog.cpp
        GUI/ScreenLoadGame/screen_load_game.cpp GUI/ScreenLoadGame/screen_load_game.h
        GameState/Pos.h
        GameState/GameState.cpp GameState/GameState.h GameState/Map.cpp GameState/Map.h GameState/Ghost.cpp GameState/Ghost.h)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pacman
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET pacman APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    if (ANDROID)
        add_library(pacman SHARED
                ${PROJECT_SOURCES}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(pacman
                ${PROJECT_SOURCES}
                )
    endif ()
endif ()

target_link_libraries(pacman PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(pacman PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS pacman
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pacman)
endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()


