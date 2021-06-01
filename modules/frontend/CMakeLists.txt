find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME}_frontend STATIC
        src/bills_window.cpp
        src/bills_window.ui)
target_include_directories(${PROJECT_NAME}_frontend PUBLIC
        src)
target_link_libraries(${PROJECT_NAME}_frontend
        PUBLIC
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
            ${PROJECT_NAME}_backend
        )
