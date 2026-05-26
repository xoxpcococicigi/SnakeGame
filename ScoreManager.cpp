#include "ScoreManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

ScoreManager::ScoreManager(const std::string& filePath)
    : m_filePath(filePath)
{
}

void ScoreManager::sortRecords()
{
    // Сортировка по убыванию очков
    std::sort(m_records.begin(), m_records.end(),
        [](const ScoreRecord& a, const ScoreRecord& b)
        {
            return a.score > b.score;
        });

    // Оставляем только топ-10
    if (m_records.size() > m_maxRecords)
    {
        m_records.resize(m_maxRecords);
    }
}

bool ScoreManager::loadRecords()
{
    m_records.clear();

    std::ifstream file(m_filePath);
    if (!file.is_open())
    {
        return false;  // Файл не существует - это нормально
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Формат: score:name
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos)
        {
            try
            {
                int score = std::stoi(line.substr(0, colonPos));
                std::string name = line.substr(colonPos + 1);
                m_records.emplace_back(name, score);
            }
            catch (...)
            {
                // Пропускаем некорректные строки
                continue;
            }
        }
    }

    sortRecords();
    return true;
}

bool ScoreManager::saveRecords() const
{
    std::ofstream file(m_filePath);
    if (!file.is_open())
    {
        return false;
    }

    for (const auto& record : m_records)
    {
        file << record.score << ":" << record.name << "\n";
    }

    return file.good();
}

void ScoreManager::addRecord(const std::string& name, int score)
{
    m_records.emplace_back(name, score);
    sortRecords();
}

std::vector<ScoreRecord> ScoreManager::getTopRecords(size_t count) const
{
    std::vector<ScoreRecord> result;
    size_t actualCount = std::min(count, m_records.size());

    for (size_t i = 0; i < actualCount; ++i)
    {
        result.push_back(m_records[i]);
    }

    return result;
}

bool ScoreManager::isHighScore(int score) const
{
    if (m_records.empty())
    {
        return true;  // Если нет записей, любой счет попадает в топ
    }

    if (m_records.size() < m_maxRecords)
    {
        return true;  // Если меньше 10 записей, всегда попадаем
    }

    // Проверяем, больше ли score последнего в топ-10
    return score > m_records.back().score;
}

void ScoreManager::clearRecords()
{
    m_records.clear();
}

size_t ScoreManager::getRecordCount() const
{
    return m_records.size();
}
