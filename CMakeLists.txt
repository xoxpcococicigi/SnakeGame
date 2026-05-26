cmake_minimum_required(VERSION 3.10)
project(SnakeGame VERSION 1.0 LANGUAGES CXX)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Исходные файлы
set(SOURCES
    main.cpp
    Game.cpp
    Snake.cpp
    Food.cpp
    SaveManager.cpp
    ScoreManager.cpp
    TextureManager.cpp
)

# Заголовочные файлы
set(HEADERS
    Game.h
    Snake.h
    Food.h
    SaveManager.h
    ScoreManager.h
    TextureManager.h
    GameState.h
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Подключение заголовочных файлов текущей директории
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Линковка с SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Копирование ресурсов после сборки
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/Saves
)
