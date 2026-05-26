#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/**
 * @brief Направления движения змейки
 */
enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @brief Класс, представляющий змейку в игре
 * 
 * Отвечает за:
 * - Хранение позиций всех сегментов тела
 * - Движение в заданном направлении
 * - Рост при поедании еды
 * - Проверку столкновений
 * - Отрисовку с использованием текстур
 * 
 * Использует инкапсуляцию: все данные скрыты в private секции
 */
class Snake
{
private:
    std::vector<sf::Vector2f> m_body;      ///< Позиции сегментов тела
    Direction m_direction;                  ///< Текущее направление движения
    Direction m_nextDirection;              ///< Следующее направление (для плавности управления)
    float m_segmentSize;                    ///< Размер одного сегмента
    bool m_grow;                            ///< Флаг роста после поедания
    sf::Texture* m_texture;                 ///< Текстура для отрисовки

public:
    /**
     * @brief Конструктор змейки
     * @param startX Начальная позиция X
     * @param startY Начальная позиция Y
     * @param segmentSize Размер сегмента
     * @param texture Текстура для отрисовки
     */
    Snake(float startX, float startY, float segmentSize, sf::Texture* texture);

    /**
     * @brief Обновить состояние змейки (движение)
     */
    void update();

    /**
     * @brief Отрисовать змейку
     * @param window Окно для отрисовки
     */
    void render(sf::RenderWindow& window);

    /**
     * @brief Изменить направление движения
     * @param direction Новое направление
     */
    void setDirection(Direction direction);

    /**
     * @brief Получить текущее направление
     * @return Текущее направление
     */
    Direction getDirection() const;

    /**
     * @brief Установить флаг роста
     */
    void setGrow(bool grow);

    /**
     * @brief Проверить столкновение со стенами
     * @param windowWidth Ширина окна
     * @param windowHeight Высота окна
     * @return true если произошло столкновение
     */
    bool checkWallCollision(float windowWidth, float windowHeight) const;

    /**
     * @brief Проверить столкновение с собой
     * @return true если произошло столкновение
     */
    bool checkSelfCollision() const;

    /**
     * @brief Получить позицию головы
     * @return Позиция головы
     */
    sf::Vector2f getHeadPosition() const;

    /**
     * @brief Проверить, занимает ли змейка данную позицию
     * @param pos Позиция для проверки
     * @return true если позиция занята
     */
    bool occupiesPosition(const sf::Vector2f& pos) const;

    /**
     * @brief Получить длину змейки
     * @return Количество сегментов
     */
    int getLength() const;

    /**
     * @brief Сериализовать состояние змейки в строку
     * @return Строка с данными для сохранения
     */
    std::string serialize() const;

    /**
     * @brief Десериализовать состояние змейки из строки
     * @param data Строка с данными
     * @param texture Текстура для отрисовки
     */
    void deserialize(const std::string& data, sf::Texture* texture);
};

#endif // SNAKE_H
