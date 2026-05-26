#include "SaveManager.h"
#include <fstream>
#include <sstream>

SaveManager::SaveManager(const std::string& filePath)
    : m_saveFilePath(filePath)
{
}

std::string SaveManager::readFile(const std::string& filename) const
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool SaveManager::writeFile(const std::string& filename, const std::string& content) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    file << content;
    return file.good();
}

bool SaveManager::saveGame(const std::string& snakeData, const std::string& foodData, int score)
{
    // Формируем JSON вручную (без внешних библиотек)
    std::ostringstream json;
    json << "{\n";
    json << "  \"snake\": \"" << snakeData << "\",\n";
    json << "  \"food\": \"" << foodData << "\",\n";
    json << "  \"score\": " << score << "\n";
    json << "}\n";

    return writeFile(m_saveFilePath, json.str());
}

bool SaveManager::loadGame(std::string& snakeData, std::string& foodData, int& score)
{
    std::string content = readFile(m_saveFilePath);
    if (content.empty())
    {
        return false;
    }

    // Простой парсинг JSON (для нашего формата)
    // Ищем ключи и извлекаем значения
    
    // Извлекаем snake data
    size_t snakePos = content.find("\"snake\":");
    if (snakePos == std::string::npos) return false;
    
    size_t snakeStart = content.find('"', snakePos + 8);
    size_t snakeEnd = content.find('"', snakeStart + 1);
    if (snakeStart == std::string::npos || snakeEnd == std::string::npos) return false;
    snakeData = content.substr(snakeStart + 1, snakeEnd - snakeStart - 1);

    // Извлекаем food data
    size_t foodPos = content.find("\"food\":");
    if (foodPos == std::string::npos) return false;
    
    size_t foodStart = content.find('"', foodPos + 7);
    size_t foodEnd = content.find('"', foodStart + 1);
    if (foodStart == std::string::npos || foodEnd == std::string::npos) return false;
    foodData = content.substr(foodStart + 1, foodEnd - foodStart - 1);

    // Извлекаем score
    size_t scorePos = content.find("\"score\":");
    if (scorePos == std::string::npos) return false;
    
    size_t scoreStart = scorePos + 8;
    while (scoreStart < content.size() && (content[scoreStart] == ' ' || content[scoreStart] == '\t'))
    {
        ++scoreStart;
    }
    
    size_t scoreEnd = scoreStart;
    while (scoreEnd < content.size() && (isdigit(content[scoreEnd]) || content[scoreEnd] == '-'))
    {
        ++scoreEnd;
    }
    
    if (scoreStart == scoreEnd) return false;
    
    try
    {
        score = std::stoi(content.substr(scoreStart, scoreEnd - scoreStart));
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool SaveManager::hasSaveFile() const
{
    std::ifstream file(m_saveFilePath);
    return file.good();
}

bool SaveManager::deleteSaveFile() const
{
    try
    {
        return std::remove(m_saveFilePath.c_str()) == 0;
    }
    catch (...)
    {
        return false;
    }
}
