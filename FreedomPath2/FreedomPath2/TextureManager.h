#pragma once
#include <windows.h>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>

class TextureManager {
public:
    TextureManager();
    ~TextureManager();  // Добавлено объявление деструктора
    void initialize();

    void drawMenuBackground(HDC hdc);
    void drawGameBackground(HDC hdc);
    void drawPlayer(HDC hdc, int x, int y);
    void drawZombie(HDC hdc, int x, int y);
    void drawDoor(HDC hdc, int x, int y, bool requiresKey);
    void drawTrap(HDC hdc, int x, int y);
    void drawCoin(HDC hdc, int x, int y);

private:
    std::mt19937 rng;
    float animationTime;

    COLORREF getRandomVariant(COLORREF baseColor, int variation);
};