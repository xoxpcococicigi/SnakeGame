#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <string>
#include <vector>
#include <utility>

/**
 * @brief Структура для хранения записи о рекорде
 */
struct ScoreRecord
{
    std::string name;  ///< Имя игрока
    int score;   ///< Очки

	ScoreRecord() : name(""), score(0) 
    {}

    ScoreRecord(const std::string& n, int s) : name(n), score(s) {}
};

/**
 * @brief Класс для управления таблицей рекордов
 * 
 * Отвечает за:
 * - Сохранение результатов в файл
 * - Загрузку результатов из файла
 * - Сортировку по убыванию очков
 * - Вывод топ-10 результатов
 * 
 * Использует инкапсуляцию: данные хранятся в private секции
 */
class ScoreManager
{
private:
    std::vector<ScoreRecord> m_records;  ///< Список рекордов
    std::string m_filePath;               ///< Путь к файлу рекордов
    const size_t m_maxRecords = 10;       ///< Максимальное количество записей

    /**
     * @brief Сортировать рекорды по убыванию
     */
    void sortRecords();

public:
    /**
     * @brief Конструктор
     * @param filePath Путь к файлу рекордов
     */
    explicit ScoreManager(const std::string& filePath = "Saves/records.txt");

    /**
     * @brief Загрузить рекорды из файла
     * @return true если загрузка успешна
     */
    bool loadRecords();

    /**
     * @brief Сохранить рекорды в файл
     * @return true если сохранение успешно
     */
    bool saveRecords() const;

    /**
     * @brief Добавить новый рекорд
     * @param name Имя игрока
     * @param score Очки
     */
    void addRecord(const std::string& name, int score);

    /**
     * @brief Получить топ-N рекордов
     * @param count Количество записей
     * @return Вектор с рекордами
     */
    std::vector<ScoreRecord> getTopRecords(size_t count = 10) const;

    /**
     * @brief Проверить, попадает ли score в топ-10
     * @param score Очки для проверки
     * @return true если попадает
     */
    bool isHighScore(int score) const;

    /**
     * @brief Очистить все рекорды
     */
    void clearRecords();

    /**
     * @brief Получить количество рекордов
     * @return Количество записей
     */
    size_t getRecordCount() const;
};

#endif // SCORE_MANAGER_H
