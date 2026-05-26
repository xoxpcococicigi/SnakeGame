#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Snake.h"
#include "Food.h"
#include "TextureManager.h"
#include "SaveManager.h"
#include "ScoreManager.h"
#include <string>

/**
 * @brief Главный класс игры, управляющий всем игровым процессом
 * 
 * Отвечает за:
 * - Создание и управление окном SFML
 * - Игровой цикл (обработка событий, обновление, отрисовка)
 * - Переключение между состояниями (MENU, PLAYING, GAME_OVER)
 * - Взаимодействие всех компонентов игры
 * 
 * Использует инкапсуляцию: все компоненты скрыты в private секции
 */
class Game
{
private:
    // Окно и основные параметры
    const int m_windowWidth = 800;          ///< Ширина окна (должна инициализироваться до m_window)
    const int m_windowHeight = 600;         ///< Высота окна (должна инициализироваться до m_window)
    sf::RenderWindow m_window;              ///< Окно игры
    const float m_gridSize = 32.0f;         ///< Размер клетки сетки

    // Состояние игры
    GameState m_currentState;               ///< Текущее состояние
    int m_score;                            ///< Текущие очки
    bool m_gameLoaded;                      ///< Флаг загруженной игры

    // Компоненты игры
    TextureManager m_textureManager;        ///< Менеджер текстур
    SaveManager m_saveManager;              ///< Менеджер сохранений
    ScoreManager m_scoreManager;            ///< Менеджер рекордов
    
    Snake* m_snake;                         ///< Змейка
    Food* m_food;                           ///< Еда

    // Графические элементы UI
    sf::Font m_font;                        ///< Шрифт для текста
    sf::Text m_scoreText;                   ///< Текст очков
    sf::RectangleShape m_startButton;       ///< Кнопка старта
    sf::Text m_startButtonText;             ///< Текст кнопки старта
    sf::Text m_gameOverText;                ///< Текст проигрыша
    sf::Text m_restartText;                 ///< Текст рестарта
    sf::Text m_leaderboardText;             ///< Таблица рекордов

    // Таймеры
    sf::Clock m_gameClock;                  ///< Игровой таймер
    sf::Clock m_moveClock;                  ///< Таймер движения змейки
    float m_moveInterval;                   ///< Интервал движения змейки

    /**
     * @brief Инициализировать шрифты и тексты
     * @return true если успешно
     */
    bool initFonts();

    /**
     * @brief Инициализировать кнопку старта
     */
    void initStartButton();

    /**
     * @brief Загрузить текстуры
     * @return true если успешно
     */
    bool loadTextures();

    /**
     * @brief Обработать события ввода
     */
    void handleEvents();

    /**
     * @brief Обновить логику игры
     */
    void update();

    /**
     * @brief Отрисовать игру
     */
    void render();

    /**
     * @brief Отрисовать меню
     */
    void renderMenu();

    /**
     * @brief Отрисовать игровой процесс
     */
    void renderGame();

    /**
     * @brief Отрисовать экран проигрыша
     */
    void renderGameOver();

    /**
     * @brief Начать новую игру
     */
    void startNewGame();

    /**
     * @brief Завершить игру (проигрыш)
     */
    void gameOver();

    /**
     * @brief Сохранить текущую игру
     */
    void saveGame();

    /**
     * @brief Загрузить сохраненную игру
     * @return true если загрузка успешна
     */
    bool loadGame();

public:
    /**
     * @brief Конструктор игры
     */
    Game();

    /**
     * @brief Деструктор
     */
    ~Game();

    /**
     * @brief Запустить игровой цикл
     */
    void run();

    /**
     * @brief Получить текущее состояние
     * @return Текущее состояние
     */
    GameState getCurrentState() const;
};

#endif // GAME_H
