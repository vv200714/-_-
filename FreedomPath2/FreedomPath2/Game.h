#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <random>
#include "TextureManager.h"  // Включаем ПЕРВЫМ!

// Предварительные объявления чтобы избежать циклических зависимостей
class Player;
class Zombie;
class Door;
class Trap;
class Coin;

class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();

    // Геттеры
    HWND getWindow() const { return hwnd; }
    HDC getMemoryDC() const { return memDC; }
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    TextureManager& getTextureManager() { return textureManager; }

    // Состояние игры
    enum class GameState { MENU, PLAYING, GAME_OVER, WIN };
    GameState getCurrentState() const { return currentState; }
    void setGameState(GameState state) { currentState = state; }

    // Статистика
    int getScore() const { return score; }
    void addScore(int points) { score += points; }
    int getKeysCollected() const { return keysCollected; }
    void addKey() { keysCollected++; }
    Player& getPlayer();

    // Игровые объекты
    std::vector<Zombie>& getZombies() { return zombies; }
    std::vector<Door>& getDoors() { return doors; }
    std::vector<Trap>& getTraps() { return traps; }
    std::vector<Coin>& getCoins() { return coins; }

    // Управление
    void resetGame();

private:
    HWND hwnd;
    HDC hdc;
    HDC memDC;
    HBITMAP hBitmap;
    int screenWidth, screenHeight;

    GameState currentState;
    TextureManager textureManager;  // Теперь определен полностью

    // Шрифты для русского текста
    HFONT gameFont;
    HFONT titleFont;
    HFONT russianFont;

    // Игровые объекты
    Player* player;
    std::vector<Zombie> zombies;
    std::vector<Door> doors;
    std::vector<Trap> traps;
    std::vector<Coin> coins;

    // Статистика
    int score;
    int keysCollected;

    // Время
    DWORD lastUpdateTime;


    // Меню
    struct MenuButton {
        RECT rect;
        std::wstring text;  // Измените string на wstring!
        bool hovered;
    };
    std::vector<MenuButton> menuButtons;

    // Методы
    void initializeWindow();
    void initializeMenu();
    void initializeGameObjects();
    void initializeFonts();           // Добавлен метод инициализации шрифтов
    void cleanupFonts();              // Добавлен метод очистки шрифтов
    void update();
    void render();
    void handleInput();
    void handleMenuInput();
    void checkCollisions();
    void spawnEntities();

    // Отрисовка
    void renderMenu();
    void renderGame();
    void renderGameOver();
    void renderWin();
    void drawText(int x, int y, const std::string& text, COLORREF color);
    void drawRussianText(int x, int y, const std::wstring& text, COLORREF color, bool isTitle = false);  // Добавлен метод для русского текста

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};