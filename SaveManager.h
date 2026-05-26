#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>

// Forward declarations
class Snake;
class Food;

/**
 * @brief Класс для управления сохранением и загрузкой состояния игры
 * 
 * Отвечает за:
 * - Сохранение состояния игры в JSON-файл
 * - Загрузку состояния из JSON-файла
 * - Сериализацию/десериализацию данных змейки, еды и очков
 * 
 * Использует инкапсуляцию: путь к файлу хранится в private секции
 */
class SaveManager
{
private:
    std::string m_saveFilePath;  ///< Путь к файлу сохранения

    /**
     * @brief Вспомогательная функция для чтения всего файла
     * @param filename Имя файла
     * @return Содержимое файла
     */
    std::string readFile(const std::string& filename) const;

    /**
     * @brief Вспомогательная функция для записи в файл
     * @param filename Имя файла
     * @param content Содержимое
     * @return true если запись успешна
     */
    bool writeFile(const std::string& filename, const std::string& content) const;

public:
    /**
     * @brief Конструктор
     * @param filePath Путь к файлу сохранения
     */
    explicit SaveManager(const std::string& filePath = "Saves/save.json");

    /**
     * @brief Сохранить состояние игры
     * @param snakeData Сериализованные данные змейки
     * @param foodData Сериализованные данные еды
     * @param score Текущие очки
     * @return true если сохранение успешно
     */
    bool saveGame(const std::string& snakeData, const std::string& foodData, int score);

    /**
     * @brief Загрузить состояние игры
     * @param snakeData Ссылка для записи данных змейки
     * @param foodData Ссылка для записи данных еды
     * @param score Ссылка для записи очков
     * @return true если загрузка успешна
     */
    bool loadGame(std::string& snakeData, std::string& foodData, int& score);

    /**
     * @brief Проверить наличие файла сохранения
     * @return true если файл существует
     */
    bool hasSaveFile() const;

    /**
     * @brief Удалить файл сохранения
     * @return true если удаление успешно
     */
    bool deleteSaveFile() const;
};

#endif // SAVE_MANAGER_H
