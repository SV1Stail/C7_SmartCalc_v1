Cmake_minimum_required(VERSION 3.1)

project(calc VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)

#backend sources
file(GLOB C_SOURCES
   "${CMAKE_SOURCE_DIR}/../backend/*.c"
   "${CMAKE_SOURCE_DIR}/../backend/help_functions/*.c"
   "${CMAKE_SOURCE_DIR}/../backend/user_data_types/*.c"
        )

add_library(calc.a STATIC ${C_SOURCES})
# front sources
set(PROJECT_SOURCES
        main.cpp
        calc.cpp
        calc.h
        calc.ui
        qcustomplot.cpp
        qcustomplot.h
        credit_calc.h
        credit_calc.cpp
        credit_calc.ui
        parser.h
        parser.c
        depos.cpp
        depos.h
        depos.ui
)

# Add the path to the QCustomPlot source code
set(QCUSTOMPLOT_SOURCE_DIR qcustomplot)

# Include the QCustomPlot directory
include_directories(${QCUSTOMPLOT_SOURCE_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(calc
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET calc APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(calc SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(calc
            ${PROJECT_SOURCES}
        )
    endif()
endif()
# Link the calc target with Qt Widgets
target_link_libraries(calc PRIVATE calc.a)

target_link_libraries(calc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(calc PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)



set_target_properties(calc PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calc)
endif()
