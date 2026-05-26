#include "Food.h"
#include <sstream>
#include <cmath>

Food::Food(float x, float y, float size, sf::Texture* texture)
    : m_position(x, y)
    , m_size(size)
    , m_texture(texture)
{
}

Food::Food()
    : m_position(0, 0)
    , m_size(32.0f)
    , m_texture(nullptr)
{
}

void Food::render(sf::RenderWindow& window)
{
    if (!m_texture) return;

    sf::Sprite sprite(*m_texture);
    sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(m_size), static_cast<int>(m_size)));
    sprite.setPosition(m_position.x, m_position.y);
    window.draw(sprite);
}

void Food::randomizePosition(float windowWidth, float windowHeight, float gridSize)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int cols = static_cast<int>(windowWidth / gridSize);
    int rows = static_cast<int>(windowHeight / gridSize);
    
    std::uniform_int_distribution<> distX(0, cols - 1);
    std::uniform_int_distribution<> distY(0, rows - 1);
    
    m_position.x = distX(gen) * gridSize;
    m_position.y = distY(gen) * gridSize;
}

void Food::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

sf::Vector2f Food::getPosition() const
{
    return m_position;
}

float Food::getSize() const
{
    return m_size;
}

void Food::setTexture(sf::Texture* texture)
{
    m_texture = texture;
}

bool Food::checkCollision(const sf::Vector2f& pos) const
{
    return (std::abs(pos.x - m_position.x) < m_size / 2 &&
            std::abs(pos.y - m_position.y) < m_size / 2);
}

std::string Food::serialize() const
{
    std::ostringstream oss;
    // Формат: x,y|size
    oss << m_position.x << "," << m_position.y << "|" << m_size;
    return oss.str();
}

void Food::deserialize(const std::string& data, sf::Texture* texture)
{
    m_texture = texture;
    
    std::istringstream iss(data);
    std::string posStr, sizeStr;
    
    if (!std::getline(iss, posStr, '|')) return;
    if (!std::getline(iss, sizeStr)) return;
    
    size_t commaPos = posStr.find(',');
    if (commaPos != std::string::npos)
    {
        float x = std::stof(posStr.substr(0, commaPos));
        float y = std::stof(posStr.substr(commaPos + 1));
        m_position.x = x;
        m_position.y = y;
    }
    
    m_size = std::stof(sizeStr);
}
