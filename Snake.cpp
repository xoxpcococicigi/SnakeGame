#include "Snake.h"
#include <sstream>
#include <cmath>

Snake::Snake(float startX, float startY, float segmentSize, sf::Texture* texture)
    : m_direction(Direction::RIGHT)
    , m_nextDirection(Direction::RIGHT)
    , m_segmentSize(segmentSize)
    , m_grow(false)
    , m_texture(texture)
{
    // Создаем начальную змейку из 3 сегментов
    m_body.push_back(sf::Vector2f(startX, startY));
    m_body.push_back(sf::Vector2f(startX - segmentSize, startY));
    m_body.push_back(sf::Vector2f(startX - segmentSize * 2, startY));
}

void Snake::update()
{
    // Применяем следующее направление
    m_direction = m_nextDirection;

    // Вычисляем новую позицию головы
    sf::Vector2f head = m_body[0];
    
    switch (m_direction)
    {
        case Direction::UP:
            head.y -= m_segmentSize;
            break;
        case Direction::DOWN:
            head.y += m_segmentSize;
            break;
        case Direction::LEFT:
            head.x -= m_segmentSize;
            break;
        case Direction::RIGHT:
            head.x += m_segmentSize;
            break;
    }

    // Добавляем новую голову в начало
    m_body.insert(m_body.begin(), head);

    // Если не растем, удаляем хвост
    if (!m_grow)
    {
        m_body.pop_back();
    }
    else
    {
        m_grow = false;  // Сбрасываем флаг роста
    }
}

void Snake::render(sf::RenderWindow& window)
{
    if (m_texture)
    {
        sf::Sprite sprite(*m_texture);

        sprite.setTextureRect(
            sf::IntRect(
                0,
                0,
                static_cast<int>(m_segmentSize),
                static_cast<int>(m_segmentSize)
            )
        );

        for (const auto& segment : m_body)
        {
            sprite.setPosition(segment.x, segment.y);
            window.draw(sprite);
        }
    }
    else
    {
        // Рисуем квадраты если текстуры нет
        for (size_t i = 0; i < m_body.size(); i++)
        {
            sf::RectangleShape rect(
                sf::Vector2f(m_segmentSize, m_segmentSize)
            );

            rect.setPosition(m_body[i]);

            if (i == 0)
                rect.setFillColor(sf::Color(0, 220, 0));
            else
                rect.setFillColor(sf::Color(0, 160, 0));

            rect.setOutlineThickness(1);
            rect.setOutlineColor(sf::Color::Black);

            window.draw(rect);
        }
    }
}

void Snake::setDirection(Direction direction)
{
    // Запрещаем разворот на 180 градусов
    bool isOpposite = false;
    
    switch (m_direction)
    {
        case Direction::UP:
            isOpposite = (direction == Direction::DOWN);
            break;
        case Direction::DOWN:
            isOpposite = (direction == Direction::UP);
            break;
        case Direction::LEFT:
            isOpposite = (direction == Direction::RIGHT);
            break;
        case Direction::RIGHT:
            isOpposite = (direction == Direction::LEFT);
            break;
    }

    if (!isOpposite)
    {
        m_nextDirection = direction;
    }
}

Direction Snake::getDirection() const
{
    return m_direction;
}

void Snake::setGrow(bool grow)
{
    m_grow = grow;
}

bool Snake::checkWallCollision(float windowWidth, float windowHeight) const
{
    sf::Vector2f head = m_body[0];
    
    return (head.x < 0 || 
            head.x >= windowWidth || 
            head.y < 0 || 
            head.y >= windowHeight);
}

bool Snake::checkSelfCollision() const
{
    if (m_body.size() < 4) return false;  // Минимальная длина для самопересечения
    
    sf::Vector2f head = m_body[0];
    
    // Проверяем столкновение головы с любым сегментом тела (кроме самой головы)
    for (size_t i = 1; i < m_body.size(); ++i)
    {
        if (std::abs(head.x - m_body[i].x) < m_segmentSize / 2 &&
            std::abs(head.y - m_body[i].y) < m_segmentSize / 2)
        {
            return true;
        }
    }
    
    return false;
}

sf::Vector2f Snake::getHeadPosition() const
{
    return m_body.empty() ? sf::Vector2f(0, 0) : m_body[0];
}

bool Snake::occupiesPosition(const sf::Vector2f& pos) const
{
    for (const auto& segment : m_body)
    {
        if (std::abs(segment.x - pos.x) < m_segmentSize / 2 &&
            std::abs(segment.y - pos.y) < m_segmentSize / 2)
        {
            return true;
        }
    }
    return false;
}

int Snake::getLength() const
{
    return static_cast<int>(m_body.size());
}

std::string Snake::serialize() const
{
    std::ostringstream oss;
    
    // Формат: direction|segmentSize|x1,y1;x2,y2;x3,y3;...
    oss << static_cast<int>(m_direction) << "|"
        << m_segmentSize << "|";
    
    for (size_t i = 0; i < m_body.size(); ++i)
    {
        oss << m_body[i].x << "," << m_body[i].y;
        if (i < m_body.size() - 1)
        {
            oss << ";";
        }
    }
    
    return oss.str();
}

void Snake::deserialize(const std::string& data, sf::Texture* texture)
{
    m_texture = texture;
    m_body.clear();
    
    std::istringstream iss(data);
    std::string directionStr, segmentSizeStr, positionsStr;
    
    // Разбираем формат: direction|segmentSize|x1,y1;x2,y2;...
    if (!std::getline(iss, directionStr, '|')) return;
    if (!std::getline(iss, segmentSizeStr, '|')) return;
    if (!std::getline(iss, positionsStr)) return;
    
    m_direction = static_cast<Direction>(std::stoi(directionStr));
    m_nextDirection = m_direction;
    m_segmentSize = std::stof(segmentSizeStr);
    
    // Разбираем позиции сегментов
    std::istringstream posStream(positionsStr);
    std::string segmentStr;
    
    while (std::getline(posStream, segmentStr, ';'))
    {
        size_t commaPos = segmentStr.find(',');
        if (commaPos != std::string::npos)
        {
            float x = std::stof(segmentStr.substr(0, commaPos));
            float y = std::stof(segmentStr.substr(commaPos + 1));
            m_body.push_back(sf::Vector2f(x, y));
        }
    }
}
