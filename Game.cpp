#include "Game.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(m_windowWidth, m_windowHeight), "Snake Game", sf::Style::Titlebar | sf::Style::Close)
    , m_currentState(GameState::MENU)
    , m_score(0)
    , m_gameLoaded(false)
    , m_snake(nullptr)
    , m_food(nullptr)
    , m_moveInterval(0.15f)  // Скорость движения змейки (секунды между ходами)
{
    m_window.setFramerateLimit(60);

    // Загружаем рекорды
    m_scoreManager.loadRecords();

    // Инициализируем шрифты и текстуры
    if (!initFonts())
    {
        std::cerr << "Warning: Could not load fonts. Using default." << std::endl;
    }

    if (!loadTextures())
    {
        std::cerr << "Warning: Could not load textures. Game will use colored rectangles." << std::endl;
    }

    initStartButton();

    // Настраиваем текст очков
    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(24);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition(10, 10);
}

Game::~Game()
{
    delete m_snake;
    delete m_food;
}

bool Game::initFonts()
{
    // Пытаемся загрузить системный шрифт или используем встроенный
    // В реальном проекте здесь был бы путь к файлу шрифта
    if (!m_font.loadFromFile("Resources/arial.ttf"))
    {
        // Пробуем альтернативные пути
        if (!m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") &&
            !m_font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
        {
            return false;
        }
    }
    return true;
}

void Game::initStartButton()
{
    m_startButton.setSize(sf::Vector2f(200, 60));
    m_startButton.setFillColor(sf::Color(100, 200, 100));
    m_startButton.setOutlineColor(sf::Color::White);
    m_startButton.setOutlineThickness(2);
    m_startButton.setPosition((m_windowWidth - 200) / 2, (m_windowHeight - 60) / 2);

    m_startButtonText.setFont(m_font);
    m_startButtonText.setString("Начать игру");
    m_startButtonText.setCharacterSize(28);
    m_startButtonText.setFillColor(sf::Color::White);
    
    sf::FloatRect textBounds = m_startButtonText.getLocalBounds();
    m_startButtonText.setOrigin(textBounds.left + textBounds.width / 2, 
                                 textBounds.top + textBounds.height / 2);
    m_startButtonText.setPosition(m_startButton.getPosition().x + 100, 
                                   m_startButton.getPosition().y + 30);

    // Настраиваем текст проигрыша
    m_gameOverText.setFont(m_font);
    m_gameOverText.setString("Игра окончена!");
    m_gameOverText.setCharacterSize(48);
    m_gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect gameOverBounds = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin(gameOverBounds.left + gameOverBounds.width / 2, 
                              gameOverBounds.top + gameOverBounds.height / 2);
    m_gameOverText.setPosition(m_windowWidth / 2, m_windowHeight / 2 - 50);

    // Настраиваем текст рестарта
    m_restartText.setFont(m_font);
    m_restartText.setString("Нажмите R для рестарта или M для меню");
    m_restartText.setCharacterSize(24);
    m_restartText.setFillColor(sf::Color::White);
    sf::FloatRect restartBounds = m_restartText.getLocalBounds();
    m_restartText.setOrigin(restartBounds.left + restartBounds.width / 2, 
                             restartBounds.top + restartBounds.height / 2);
    m_restartText.setPosition(m_windowWidth / 2, m_windowHeight / 2 + 20);

    // Настраиваем текст таблицы рекордов
    m_leaderboardText.setFont(m_font);
    m_leaderboardText.setCharacterSize(20);
    m_leaderboardText.setFillColor(sf::Color::Yellow);
    m_leaderboardText.setPosition(10, m_windowHeight - 200);
}

bool Game::loadTextures()
{
    bool success = true;

    // Загружаем текстуры (если файлы существуют)
    if (!m_textureManager.loadTexture("snake", "Resources/snake.png"))
    {
        success = false;
    }
    if (!m_textureManager.loadTexture("apple", "Resources/apple.png"))
    {
        success = false;
    }
    if (!m_textureManager.loadTexture("background", "Resources/background.png"))
    {
        success = false;
    }

    return success;
}

void Game::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        if (m_currentState == GameState::MENU)
        {
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = m_window.mapPixelToCoords(
                        sf::Mouse::getPosition(m_window));
                    
                    if (m_startButton.getGlobalBounds().contains(mousePos))
                    {
                        startNewGame();
                    }
                }
            }
        }
        else if (m_currentState == GameState::PLAYING)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                // Управление WASD или стрелками
                switch (event.key.code)
                {
                    case sf::Keyboard::W:
                    case sf::Keyboard::Up:
                        if (m_snake) m_snake->setDirection(Direction::UP);
                        break;
                    case sf::Keyboard::S:
                    case sf::Keyboard::Down:
                        if (m_snake) m_snake->setDirection(Direction::DOWN);
                        break;
                    case sf::Keyboard::A:
                    case sf::Keyboard::Left:
                        if (m_snake) m_snake->setDirection(Direction::LEFT);
                        break;
                    case sf::Keyboard::D:
                    case sf::Keyboard::Right:
                        if (m_snake) m_snake->setDirection(Direction::RIGHT);
                        break;
                    case sf::Keyboard::Escape:
                        saveGame();
                        m_currentState = GameState::MENU;
                        break;
                }
            }
        }
        else if (m_currentState == GameState::GAME_OVER)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::R)
                {
                    startNewGame();
                }
                else if (event.key.code == sf::Keyboard::M)
                {
                    m_currentState = GameState::MENU;
                    delete m_snake;
                    delete m_food;
                    m_snake = nullptr;
                    m_food = nullptr;
                }
            }
        }
    }
}

void Game::update()
{
    if (m_currentState != GameState::PLAYING)
    {
        return;
    }

    // Обновляем змейку с заданным интервалом
    if (m_moveClock.getElapsedTime().asSeconds() >= m_moveInterval)
    {
        m_moveClock.restart();

        if (m_snake)
        {
            m_snake->update();

            // Проверка столкновения со стенами
            if (m_snake->checkWallCollision(m_windowWidth, m_windowHeight))
            {
                gameOver();
                return;
            }

            // Проверка столкновения с собой
            if (m_snake->checkSelfCollision())
            {
                gameOver();
                return;
            }

            // Проверка поедания еды
            if (m_food && m_food->checkCollision(m_snake->getHeadPosition()))
            {
                m_score += 10;
                m_snake->setGrow(true);
                
                // Генерируем новую еду в позиции, не занятой змейкой
                do
                {
                    m_food->randomizePosition(m_windowWidth, m_windowHeight, m_gridSize);
                } while (m_snake->occupiesPosition(m_food->getPosition()));
            }
        }
    }
}

void Game::render()
{
    m_window.clear(sf::Color::Black);

    // Отрисовка фона (если есть текстура)
    if (m_textureManager.hasTexture("background"))
    {
        sf::Sprite background(m_textureManager.getTexture("background"));
        background.setScale(
            static_cast<float>(m_windowWidth) / background.getTexture()->getSize().x,
            static_cast<float>(m_windowHeight) / background.getTexture()->getSize().y
        );
        m_window.draw(background);
    }

    switch (m_currentState)
    {
        case GameState::MENU:
            renderMenu();
            break;
        case GameState::PLAYING:
            renderGame();
            break;
        case GameState::GAME_OVER:
            renderGameOver();
            break;
        default:
            break;
    }

    m_window.display();
}

void Game::renderMenu()
{
    // Отрисовка кнопки старта
    m_window.draw(m_startButton);
    m_window.draw(m_startButtonText);

    // Отрисовка таблицы рекордов
    std::string leaderboardStr = "Топ-10 рекордов:\n";
    auto topRecords = m_scoreManager.getTopRecords(10);
    
    for (size_t i = 0; i < topRecords.size(); ++i)
    {
        leaderboardStr += std::to_string(i + 1) + ". " + 
                          topRecords[i].name + ": " + 
                          std::to_string(topRecords[i].score) + "\n";
    }

    m_leaderboardText.setString(leaderboardStr);
    m_window.draw(m_leaderboardText);

    // Подсказка о загрузке
    if (m_saveManager.hasSaveFile())
    {
        sf::Text loadText; 
        loadText.setFont(m_font);
        loadText.setString("Нажмите L для загрузки сохранения");
        loadText.setCharacterSize(20);
        loadText.setFillColor(sf::Color::Cyan);
        loadText.setPosition((m_windowWidth - loadText.getLocalBounds().width) / 2, 
                            m_windowHeight - 50);
        m_window.draw(loadText);
    }
}

void Game::renderGame()
{
    // Отрисовка очков
    m_scoreText.setString("Очки: " + std::to_string(m_score));
    m_window.draw(m_scoreText);

    // Отрисовка змейки
    if (m_snake)
    {
        sf::Texture* snakeTex = m_textureManager.hasTexture("snake") 
                                ? &m_textureManager.getTexture("snake") 
                                : nullptr;
        
        if (!snakeTex && m_snake)
        {
            // Если нет текстуры, рисуем зелеными квадратами
            for (size_t i = 0; i < m_snake->getLength(); ++i)
            {
                sf::Vector2f pos = i == 0 ? m_snake->getHeadPosition() : 
                                   m_snake->getHeadPosition(); // Упрощенно
                
                sf::RectangleShape segment(sf::Vector2f(m_gridSize, m_gridSize));
                segment.setPosition(pos.x, pos.y);
                segment.setFillColor(i == 0 ? sf::Color(0, 200, 0) : sf::Color(0, 150, 0));
                segment.setOutlineColor(sf::Color::White);
                segment.setOutlineThickness(1);
                m_window.draw(segment);
            }
        }
        else
        {
            m_snake->render(m_window);
        }
    }

    // Отрисовка еды
    if (m_food)
    {
        sf::Texture* appleTex = m_textureManager.hasTexture("apple.png") 
                                ? &m_textureManager.getTexture("apple.png") 
                                : nullptr;
        
        if (!appleTex)
        {
            // Если нет текстуры, рисуем красным квадратом
            sf::RectangleShape food(sf::Vector2f(m_gridSize, m_gridSize));
            food.setPosition(m_food->getPosition().x, m_food->getPosition().y);
            food.setFillColor(sf::Color::Red);
            food.setOutlineColor(sf::Color::White);
            food.setOutlineThickness(1);
            m_window.draw(food);
        }
        else
        {
            m_food->render(m_window);
        }
    }

    // Подсказка о сохранении
    sf::Text saveHint;
    saveHint.setFont(m_font);
    saveHint.setString("ESC - Сохранить и выйти в меню");
    saveHint.setCharacterSize(16);
    saveHint.setFillColor(sf::Color(200, 200, 200));
    saveHint.setPosition(10, m_windowHeight - 30);
    m_window.draw(saveHint);
}

void Game::renderGameOver()
{
    // Затемнение фона
    sf::RectangleShape overlay(sf::Vector2f(m_windowWidth, m_windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    m_window.draw(overlay);

    // Отрисовка текста проигрыша
    m_window.draw(m_gameOverText);
    m_window.draw(m_restartText);

    // Отображение финального счета
    sf::Text finalScore;
    finalScore.setFont(m_font); 
    finalScore.setString ("Ваш счет: " + std::to_string(m_score));
    finalScore.setCharacterSize(32);
    finalScore.setFillColor(sf::Color::White);
    sf::FloatRect scoreBounds = finalScore.getLocalBounds();
    finalScore.setOrigin(scoreBounds.left + scoreBounds.width / 2, 
                         scoreBounds.top + scoreBounds.height / 2);
    finalScore.setPosition(m_windowWidth / 2, m_windowHeight / 2 + 70);
    m_window.draw(finalScore);

    // Проверка на новый рекорд
    if (m_scoreManager.isHighScore(m_score))
    {
        sf::Text newRecord;
        newRecord.setFont(m_font);
        newRecord.setString("Новый рекорд!");
        newRecord.setCharacterSize(28);
        newRecord.setFillColor(sf::Color::Yellow);
        sf::FloatRect recordBounds = newRecord.getLocalBounds();
        newRecord.setOrigin(recordBounds.left + recordBounds.width / 2, 
                            recordBounds.top + recordBounds.height / 2);
        newRecord.setPosition(m_windowWidth / 2, m_windowHeight / 2 + 110);
        m_window.draw(newRecord);
    }
}

void Game::startNewGame()
{
    // Очищаем старые объекты
    delete m_snake;
    delete m_food;

    // Создаем новую змейку
    sf::Texture* snakeTex = m_textureManager.hasTexture("snake.png") 
                            ? &m_textureManager.getTexture("snake.png") 
                            : nullptr;
    m_snake = new Snake(m_windowWidth / 2, m_windowHeight / 2, m_gridSize, snakeTex);

    // Создаем еду
    sf::Texture* appleTex = m_textureManager.hasTexture("apple.png") 
                            ? &m_textureManager.getTexture("apple.png") 
                            : nullptr;
    m_food = new Food(100, 100, m_gridSize, appleTex);
    
    // Перемещаем еду в случайную позицию
    m_food->randomizePosition(m_windowWidth, m_windowHeight, m_gridSize);

    // Сбрасываем счет
    m_score = 0;
    m_gameLoaded = false;
    m_moveClock.restart();

    // Переключаем состояние
    m_currentState = GameState::PLAYING;
}

void Game::gameOver()
{
    // Сохраняем рекорд
    m_scoreManager.addRecord("Player", m_score);
    m_scoreManager.saveRecords();

    // Удаляем файл сохранения текущей игры
    m_saveManager.deleteSaveFile();

    m_currentState = GameState::GAME_OVER;
}

void Game::saveGame()
{
    if (!m_snake || !m_food) return;

    std::string snakeData = m_snake->serialize();
    std::string foodData = m_food->serialize();

    if (m_saveManager.saveGame(snakeData, foodData, m_score))
    {
        std::cout << "Game saved successfully!" << std::endl;
    }
    else
    {
        std::cerr << "Failed to save game!" << std::endl;
    }
}

bool Game::loadGame()
{
    std::string snakeData, foodData;
    int score;

    if (!m_saveManager.loadGame(snakeData, foodData, score))
    {
        return false;
    }

    // Очищаем старые объекты
    delete m_snake;
    delete m_food;

    // Восстанавливаем змейку
    sf::Texture* snakeTex = m_textureManager.hasTexture("snake") 
                            ? &m_textureManager.getTexture("snake") 
                            : nullptr;
    m_snake = new Snake(0, 0, m_gridSize, snakeTex);
    m_snake->deserialize(snakeData, snakeTex);

    // Восстанавливаем еду
    sf::Texture* appleTex = m_textureManager.hasTexture("apple") 
                            ? &m_textureManager.getTexture("apple") 
                            : nullptr;
    m_food = new Food(0, 0, m_gridSize, appleTex);
    m_food->deserialize(foodData, appleTex);

    // Восстанавливаем счет
    m_score = score;
    m_gameLoaded = true;
    m_moveClock.restart();

    m_currentState = GameState::PLAYING;
    return true;
}

void Game::run()
{
    while (m_window.isOpen())
    {
        handleEvents();
        update();
        render();

        // Обработка загрузки по клавише L в меню
        if (m_currentState == GameState::MENU)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && m_saveManager.hasSaveFile())
            {
                if (loadGame())
                {
                    std::cout << "Game loaded successfully!" << std::endl;
                }
                // Небольшая задержка чтобы избежать повторного срабатывания
                sf::sleep(sf::milliseconds(200));
            }
        }
    }
}

GameState Game::getCurrentState() const
{
    return m_currentState;
}
