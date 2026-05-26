#ifndef FOOD_H
#define FOOD_H

#include <SFML/Graphics.hpp>
#include <random>

/**
 * @brief Класс, представляющий еду (яблоко) в игре
 * 
 * Отвечает за:
 * - Хранение позиции еды
 * - Генерацию случайной позиции
 * - Отрисовку с использованием текстур
 * 
 * Использует инкапсуляцию: позиция и текстура скрыты в private секции
 */
class Food
{
private:
    sf::Vector2f m_position;      ///< Позиция еды
    float m_size;                  ///< Размер еды
    sf::Texture* m_texture;        ///< Текстура для отрисовки

public:
    /**
     * @brief Конструктор еды
     * @param x Начальная позиция X
     * @param y Начальная позиция Y
     * @param size Размер еды
     * @param texture Текстура для отрисовки
     */
    Food(float x, float y, float size, sf::Texture* texture);

    /**
     * @brief Конструктор по умолчанию (для загрузки)
     */
    Food();

    /**
     * @brief Отрисовать еду
     * @param window Окно для отрисовки
     */
    void render(sf::RenderWindow& window);

    /**
     * @brief Переместить еду в случайную позицию
     * @param windowWidth Ширина окна
     * @param windowHeight Высота окна
     * @param gridSize Размер сетки (должен быть кратен size)
     */
    void randomizePosition(float windowWidth, float windowHeight, float gridSize);

    /**
     * @brief Установить позицию еды
     * @param x Позиция X
     * @param y Позиция Y
     */
    void setPosition(float x, float y);

    /**
     * @brief Получить позицию еды
     * @return Позиция еды
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Получить размер еды
     * @return Размер
     */
    float getSize() const;

    /**
     * @brief Установить текстуру
     * @param texture Текстура
     */
    void setTexture(sf::Texture* texture);

    /**
     * @brief Проверить столкновение с заданной позицией
     * @param pos Позиция для проверки
     * @return true если позиция совпадает
     */
    bool checkCollision(const sf::Vector2f& pos) const;

    /**
     * @brief Сериализовать состояние еды в строку
     * @return Строка с данными для сохранения
     */
    std::string serialize() const;

    /**
     * @brief Десериализовать состояние еды из строки
     * @param data Строка с данными
     * @param texture Текстура для отрисовки
     */
    void deserialize(const std::string& data, sf::Texture* texture);
};

#endif // FOOD_H
