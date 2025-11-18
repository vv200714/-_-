#include "Wall.h"
#include "Game.h"

Wall::Wall() :
    game(nullptr), x(0), y(0), width(32), height(32), solid(true) {
}

void Wall::initialize(Game* gameInstance) {
    game = gameInstance;
    solid = true;
}

void Wall::render() {
    if (game) {
        // Рисуем стену как серый прямоугольник с текстурой камня
        HBRUSH wallBrush = CreateSolidBrush(RGB(80, 80, 80));
        RECT wallRect = {
            static_cast<int>(x),
            static_cast<int>(y),
            static_cast<int>(x + width),
            static_cast<int>(y + height)
        };
        FillRect(game->getMemoryDC(), &wallRect, wallBrush);
        DeleteObject(wallBrush);

        // Добавляем текстуру камня
        HPEN stonePen = CreatePen(PS_SOLID, 1, RGB(60, 60, 60));
        HPEN oldPen = (HPEN)SelectObject(game->getMemoryDC(), stonePen);

        // Рисуем линии, имитирующие камни
        for (int i = 0; i < static_cast<int>(width); i += 8) {
            for (int j = 0; j < static_cast<int>(height); j += 8) {
                MoveToEx(game->getMemoryDC(), static_cast<int>(x) + i, static_cast<int>(y) + j, NULL);
                LineTo(game->getMemoryDC(), static_cast<int>(x) + i + 4, static_cast<int>(y) + j);
                LineTo(game->getMemoryDC(), static_cast<int>(x) + i + 4, static_cast<int>(y) + j + 4);
                LineTo(game->getMemoryDC(), static_cast<int>(x) + i, static_cast<int>(y) + j + 4);
                LineTo(game->getMemoryDC(), static_cast<int>(x) + i, static_cast<int>(y) + j);
            }
        }

        SelectObject(game->getMemoryDC(), oldPen);
        DeleteObject(stonePen);
    }
}

void Wall::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Wall::setSize(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
}