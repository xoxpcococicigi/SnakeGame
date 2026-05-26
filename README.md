# Snake Game - Курсовая работа по ООП

## Структура проекта

```
SnakeGame/
├── main.cpp              # Точка входа в программу
├── Game.h / Game.cpp     # Главный класс игры, управляет игровым циклом
├── Snake.h / Snake.cpp   # Класс змейки (движение, рост, столкновения)
├── Food.h / Food.cpp     # Класс еды (позиция, генерация)
├── SaveManager.h / SaveManager.cpp  # Сохранение/загрузка в JSON
├── ScoreManager.h / ScoreManager.cpp # Таблица рекордов
├── TextureManager.h / TextureManager.cpp # Управление текстурами
├── GameState.h           # Перечисление состояний игры
├── Resources/            # Папка с изображениями
│   ├── snake.png         # Текстура змейки (32x32)
│   ├── apple.png         # Текстура яблока (32x32)
│   └── background.png    # Фон (800x600)
└── Saves/                # Папка сохранений
    ├── save.json         # Файл сохранения игры
    └── records.txt       # Таблица рекордов
```

## Описание классов

### GameState (enum class)
Перечисление состояний игры: MENU, PLAYING, GAME_OVER, PAUSED.

### TextureManager
Менеджер ресурсов для загрузки и кэширования текстур.
- **Инкапсуляция**: `std::map<std::string, sf::Texture> m_textures` (private)
- **Методы**: loadTexture(), getTexture(), hasTexture(), clear()

### Snake
Класс змейки, управляющий движением и состоянием.
- **Инкапсуляция**: `m_body`, `m_direction`, `m_segmentSize` (private)
- **Методы**: update(), render(), setDirection(), checkWallCollision(), serialize()

### Food
Класс еды с генерацией случайных позиций.
- **Инкапсуляция**: `m_position`, `m_size`, `m_texture` (private)
- **Методы**: randomizePosition(), render(), checkCollision(), serialize()

### SaveManager
Управление сохранением в JSON-формате.
- **Инкапсуляция**: `m_saveFilePath` (private)
- **Методы**: saveGame(), loadGame(), hasSaveFile(), deleteSaveFile()

### ScoreManager
Таблица рекордов с сортировкой.
- **Инкапсуляция**: `m_records`, `m_filePath` (private)
- **Методы**: addRecord(), getTopRecords(), isHighScore(), saveRecords()

### Game
Главный класс, объединяющий все компоненты.
- **Инкапсуляция**: все компоненты (private)
- **Методы**: run(), handleEvents(), update(), render(), startNewGame(), gameOver()

## Управление

- **WASD** или **Стрелки** - управление змейкой
- **ESC** - сохранение и выход в меню
- **L** - загрузка сохранения (в меню)
- **R** - рестарт после проигрыша
- **M** - выход в меню после проигрыша
- **ЛКМ** - нажатие кнопки "Начать игру"

## Сборка в Visual Studio

### Шаг 1: Установка SFML

1. Скачайте SFML с официального сайта: https://www.sfml-dev.org/download.php
2. Выберите версию для Visual Studio (например, sfml-2.6.1-windows-vc17-64-bit)
3. Распакуйте архив в удобное место (например, `C:\SFML`)

### Шаг 2: Создание проекта

1. Откройте Visual Studio
2. File → New → Project → Console App (C++)
3. Назовите проект "SnakeGame"
4. Создайте папки Resources и Saves в директории проекта

### Шаг 3: Настройка путей включения

1. Project → Properties → C/C++ → General → Additional Include Directories
2. Добавьте: `C:\SFML\include`
3. Добавьте путь к заголовочным файлам проекта (если нужно)

### Шаг 4: Настройка библиотек

1. Project → Properties → Linker → General → Additional Library Directories
2. Добавьте: `C:\SFML\lib`

### Шаг 5: Подключение библиотек

1. Project → Properties → Linker → Input → Additional Dependencies
2. Добавьте следующие библиотеки:
   ```
   sfml-graphics-d.lib
   sfml-window-d.lib
   sfml-system-d.lib
   sfml-audio-d.lib   (опционально)
   sfml-network-d.lib (опционально)
   ```
   
   **Важно**: Для Release-конфигурации используйте без "-d":
   ```
   sfml-graphics.lib
   sfml-window.lib
   sfml-system.lib
   ```

### Шаг 6: Копирование DLL

Для Debug-конфигурации скопируйте из `C:\SFML\bin`:
- sfml-graphics-d-2.dll
- sfml-window-d-2.dll
- sfml-system-d-2.dll

Для Release-конфигурации:
- sfml-graphics-2.dll
- sfml-window-2.dll
- sfml-system-2.dll

Скопируйте DLL в папку с исполняемым файлом или добавьте `C:\SFML\bin` в PATH.

### Шаг 7: Добавление файлов проекта

Добавьте все .cpp и .h файлы в проект:
1. Right-click на проекте → Add → Existing Item
2. Выберите все файлы из папки SnakeGame

### Шаг 8: Настройка рабочей директории

1. Project → Properties → Debugging → Working Directory
2. Установите: `$(ProjectDir)` или путь к папке SnakeGame

### Шаг 9: Добавление ресурсов

Поместите изображения в папку Resources/:
- snake.png (рекомендуется 32x32 пикселя)
- apple.png (рекомендуется 32x32 пикселя)
- background.png (рекомендуется 800x600 пикселей)

**Примечание**: Игра работает и без текстур, используя цветные прямоугольники.

### Шаг 10: Сборка и запуск

1. Build → Build Solution (Ctrl+Shift+B)
2. Debug → Start Without Debugging (Ctrl+F5)

## Сборка через CMake (альтернатива)

Создайте файл CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(SnakeGame
    main.cpp
    Game.cpp
    Snake.cpp
    Food.cpp
    SaveManager.cpp
    ScoreManager.cpp
    TextureManager.cpp
)

target_link_libraries(SnakeGame sfml-graphics sfml-window sfml-system)
```

## Требования

- C++17 или выше
- SFML 2.5 или выше
- Компилятор: MSVC, GCC, Clang

## Особенности реализации

1. **ООП принципы**:
   - Инкапсуляция: все данные классов private
   - Разделение ответственности: каждый класс отвечает за свою функциональность
   - Нет глобальных переменных

2. **Сохранение в JSON**:
   - Формат: `{"snake": "...", "food": "...", "score": N}`
   - Сериализация состояния змейки и еды

3. **Таблица рекордов**:
   - Хранение в текстовом файле (format: score:name)
   - Автоматическая сортировка по убыванию
   - Топ-10 результатов

4. **Графика**:
   - Поддержка текстур (png)
   - Резервный режим с цветными прямоугольниками
   - Плавная анимация через игровой цикл
