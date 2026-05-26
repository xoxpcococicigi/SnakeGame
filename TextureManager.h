#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <iostream>

/**
 * @brief Класс для управления текстурами игры
 * 
 * Реализует паттерн менеджера ресурсов:
 * - Загружает текстуры из файлов при первом запросе
 * - Кэширует загруженные текстуры для повторного использования
 * - Предоставляет доступ к текстурам по имени
 * 
 * Использует инкапсуляцию: карта текстур скрыта в private секции
 */
class TextureManager
{
private:
    std::map<std::string, sf::Texture> m_textures;  ///< Карта загруженных текстур

public:
    /**
     * @brief Загрузить текстуру из файла
     * @param name Имя для идентификации текстуры
     * @param filename Путь к файлу изображения
     * @return true если загрузка успешна
     */
    bool loadTexture(const std::string& name, const std::string& filename);

    /**
     * @brief Получить ссылку на текстуру по имени
     * @param name Имя текстуры
     * @return Ссылка на текстуру
     * @throws std::runtime_error если текстура не найдена
     */
    sf::Texture& getTexture(const std::string& name);

    /**
     * @brief Проверить наличие текстуры
     * @param name Имя текстуры
     * @return true если текстура загружена
     */
    bool hasTexture(const std::string& name) const;

    /**
     * @brief Очистить все текстуры
     */
    void clear();
};

#endif // TEXTUREMANAGER_H
