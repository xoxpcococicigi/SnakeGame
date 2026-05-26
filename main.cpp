/**
 * @file main.cpp
 * @brief Точка входа в игру "Змейка"
 * 
 * Основной файл программы, создающий и запускающий игровой объект.
 * Соответствует требованиям ООП: минимальная логика в main, 
 * вся функциональность инкапсулирована в классе Game.
 */

#include "Game.h"
#include <iostream>

int main()
{
    try
    {
        // Создаем и запускаем игру
        Game game;
        game.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred!" << std::endl;
        return 1;
    }

    return 0;
}
