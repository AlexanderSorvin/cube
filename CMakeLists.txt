# ===== Версия cmake =====
cmake_minimum_required (VERSION 3.1)

# ===== Название проекта =====
project(opengl)

# ===== Модули =====
find_package(Qt5 REQUIRED
    Core
    Gui
    Opengl
    Widgets
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ===== Создание цели =====
file(GLOB_RECURSE HDRS CONFIGURE_DEPENDS
    "include/*.h"
    "include/*.hpp"
)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
    "src/*.cpp"
)

file(GLOB_RECURSE RES CONFIGURE_DEPENDS
    "*.qrc"
)

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${RES})

# ===== Свойства цели =====
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

# ===== Подключения =====
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include/
)

# ===== Библиотеки =====
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets    
    Qt5::OpenGL
)