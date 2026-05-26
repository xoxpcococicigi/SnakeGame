#include "TextureManager.h"

bool TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
    // Проверяем, загружена ли уже текстура с таким именем
    if (m_textures.find(name) != m_textures.end())
    {
        return true;  // Текстура уже загружена
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filename))
    {
        std::cerr << "Error: Could not load texture: " << filename << std::endl;
        return false;
    }

    // Сохраняем текстуру в карту
    m_textures[name] = texture;
    return true;
}

sf::Texture& TextureManager::getTexture(const std::string& name)
{
    auto it = m_textures.find(name);
    if (it == m_textures.end())
    {
        throw std::runtime_error("Texture not found: " + name);
    }
    return it->second;
}

bool TextureManager::hasTexture(const std::string& name) const
{
    return m_textures.find(name) != m_textures.end();
}

void TextureManager::clear()
{
    m_textures.clear();
}
