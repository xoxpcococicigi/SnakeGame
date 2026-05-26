#ifndef GAMESTATE_H
#define GAMESTATE_H

/**
 * @brief Перечисление состояний игры
 * 
 * Используется для управления различными экранами игры:
 * - MENU: главное меню с кнопкой старта
 * - PLAYING: активный игровой процесс
 * - GAME_OVER: экран проигрыша
 * - PAUSED: игра на паузе (для будущего расширения)
 */
enum class GameState
{
    MENU,       // Главное меню
    PLAYING,    // Игровой процесс
    GAME_OVER,  // Экран проигрыша
    PAUSED      // Пауза
};

#endif // GAMESTATE_H
