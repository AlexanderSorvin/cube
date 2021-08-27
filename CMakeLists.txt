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
set(HDRS
    ${PROJECT_SOURCE_DIR}/include/geometryengine.h
    ${PROJECT_SOURCE_DIR}/include/mainwidget.h
)

set(SRCS
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/geometryengine.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwidget.cpp
)

set(RES
    ${PROJECT_SOURCE_DIR}/shaders.qrc
    ${PROJECT_SOURCE_DIR}/textures.qrc
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