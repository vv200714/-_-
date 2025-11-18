#include "TextureManager.h"
#include <algorithm>

TextureManager::TextureManager() : animationTime(0.0f) {
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed));
}

TextureManager::~TextureManager() {
    // Деструктор - можно оставить пустым или удалить совсем
}

void TextureManager::initialize() {
    // Инициализация текстур
}

COLORREF TextureManager::getRandomVariant(COLORREF baseColor, int variation) {
    int r = GetRValue(baseColor) + (std::uniform_int_distribution<>(-variation, variation)(rng));
    int g = GetGValue(baseColor) + (std::uniform_int_distribution<>(-variation, variation)(rng));
    int b = GetBValue(baseColor) + (std::uniform_int_distribution<>(-variation, variation)(rng));

    r = (std::max)(0, (std::min)(255, r));
    g = (std::max)(0, (std::min)(255, g));
    b = (std::max)(0, (std::min)(255, b));

    return RGB(r, g, b);
}

// Остальной код без изменений...
void TextureManager::drawMenuBackground(HDC hdc) {
    RECT rect;
    GetClientRect(WindowFromDC(hdc), &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    animationTime += 0.1f;

    // Заливка темным фоном
    HBRUSH bgBrush = CreateSolidBrush(RGB(30, 20, 10));
    RECT bgRect = { 0, 0, width, height };
    FillRect(hdc, &bgRect, bgBrush);
    DeleteObject(bgBrush);

    // Рисуем мерцающие факелы поверх
    for (int y = 0; y < height; y += 8) {
        for (int x = 0; x < width; x += 8) {
            // Мерцающие огни факелов
            float torch1 = 0.3f * sinf(x * 0.02f + animationTime * 3) *
                expf(-((x - 200) * (x - 200) + (y - 150) * (y - 150)) / 8000.0f);
            float torch2 = 0.3f * cosf(x * 0.015f + animationTime * 2.5f) *
                expf(-((x - 600) * (x - 600) + (y - 150) * (y - 150)) / 9000.0f);

            float brightness = (std::min)(1.0f, torch1 + torch2);

            if (brightness > 0.1f) {
                COLORREF color = RGB(255 * brightness, 200 * brightness, 100 * brightness);
                HBRUSH brush = CreateSolidBrush(color);
                RECT pixel = { x, y, x + 4, y + 4 };
                FillRect(hdc, &pixel, brush);
                DeleteObject(brush);
            }
        }
    }
}

void TextureManager::drawGameBackground(HDC hdc) {
    RECT rect;
    GetClientRect(WindowFromDC(hdc), &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Основной фон земли
    for (int y = 0; y < height; y += 4) {
        for (int x = 0; x < width; x += 4) {
            float noise = 0.5f + 0.2f * sinf(x * 0.01f) * cosf(y * 0.01f);

            // Трещины
            float crack = 0.0f;
            if (fmodf(x + y * 0.3f, 60) < 2) {
                crack = 0.3f;
            }

            float brightness = noise - crack;
            COLORREF color = RGB(60 * brightness, 40 * brightness, 20 * brightness);

            HBRUSH brush = CreateSolidBrush(color);
            RECT pixel = { x, y, x + 4, y + 4 };
            FillRect(hdc, &pixel, brush);
            DeleteObject(brush);
        }
    }
}

void TextureManager::drawPlayer(HDC hdc, int x, int y) {
    // Тело (синяя футболка)
    HBRUSH bodyBrush = CreateSolidBrush(RGB(0, 0, 255));
    RECT bodyRect = { x, y + 4, x + 16, y + 16 };
    FillRect(hdc, &bodyRect, bodyBrush);
    DeleteObject(bodyBrush);

    // Голова
    HBRUSH headBrush = CreateSolidBrush(RGB(255, 200, 150));
    RECT headRect = { x + 4, y, x + 12, y + 8 };
    FillRect(hdc, &headRect, headBrush);
    DeleteObject(headBrush);

    // Ноги (джинсы)
    HBRUSH legsBrush = CreateSolidBrush(RGB(0, 0, 128));
    RECT legsRect = { x + 4, y + 12, x + 12, y + 16 };
    FillRect(hdc, &legsRect, legsBrush);
    DeleteObject(legsBrush);

    // Рюкзак
    HBRUSH backpackBrush = CreateSolidBrush(RGB(139, 69, 19));
    RECT backpackRect = { x + 12, y + 6, x + 15, y + 12 };
    FillRect(hdc, &backpackRect, backpackBrush);
    DeleteObject(backpackBrush);
}

void TextureManager::drawZombie(HDC hdc, int x, int y) {
    // Тело зомби
    HBRUSH bodyBrush = CreateSolidBrush(RGB(100, 120, 80));
    RECT bodyRect = { x, y + 4, x + 16, y + 16 };
    FillRect(hdc, &bodyRect, bodyBrush);
    DeleteObject(bodyBrush);

    // Голова
    HBRUSH headBrush = CreateSolidBrush(RGB(200, 220, 180));
    RECT headRect = { x + 4, y, x + 12, y + 8 };
    FillRect(hdc, &headRect, headBrush);
    DeleteObject(headBrush);

    // Красные глаза
    HBRUSH eyeBrush = CreateSolidBrush(RGB(255, 0, 0));
    RECT leftEye = { x + 5, y + 2, x + 7, y + 4 };
    RECT rightEye = { x + 9, y + 2, x + 11, y + 4 };
    FillRect(hdc, &leftEye, eyeBrush);
    FillRect(hdc, &rightEye, eyeBrush);
    DeleteObject(eyeBrush);

    // Рваная одежда
    HBRUSH clothesBrush = CreateSolidBrush(RGB(50, 50, 50));
    RECT clothesRect = { x + 2, y + 12, x + 14, y + 16 };
    FillRect(hdc, &clothesRect, clothesBrush);
    DeleteObject(clothesBrush);
}

void TextureManager::drawDoor(HDC hdc, int x, int y, bool requiresKey) {
    COLORREF doorColor = requiresKey ? RGB(60, 30, 10) : RGB(80, 40, 0);

    HBRUSH doorBrush = CreateSolidBrush(doorColor);
    RECT doorRect = { x, y, x + 16, y + 32 };
    FillRect(hdc, &doorRect, doorBrush);
    DeleteObject(doorBrush);

    // Ручка
    if (!requiresKey) {
        HBRUSH handleBrush = CreateSolidBrush(RGB(200, 200, 200));
        RECT handleRect = { x + 12, y + 16, x + 14, y + 18 };
        FillRect(hdc, &handleRect, handleBrush);
        DeleteObject(handleBrush);
    }
    else {
        // Замок для двери с ключом
        HBRUSH lockBrush = CreateSolidBrush(RGB(150, 150, 150));
        RECT lockRect = { x + 6, y + 12, x + 10, y + 20 };
        FillRect(hdc, &lockRect, lockBrush);
        DeleteObject(lockBrush);

        // Ключевое отверстие
        HBRUSH holeBrush = CreateSolidBrush(RGB(0, 0, 0));
        RECT holeRect = { x + 7, y + 16, x + 9, y + 17 };
        FillRect(hdc, &holeRect, holeBrush);
        DeleteObject(holeBrush);
    }
}

void TextureManager::drawTrap(HDC hdc, int x, int y) {
    // Основание ловушки
    HBRUSH baseBrush = CreateSolidBrush(RGB(100, 100, 100));
    RECT baseRect = { x, y + 8, x + 16, y + 16 };
    FillRect(hdc, &baseRect, baseBrush);
    DeleteObject(baseBrush);

    // Шипы
    HBRUSH spikeBrush = CreateSolidBrush(RGB(150, 150, 150));
    for (int i = 0; i < 4; ++i) {
        RECT spikeRect = { x + 2 + i * 4, y + 4, x + 6 + i * 4, y + 8 };
        FillRect(hdc, &spikeRect, spikeBrush);
    }
    DeleteObject(spikeBrush);
}

void TextureManager::drawCoin(HDC hdc, int x, int y) {
    HBRUSH coinBrush = CreateSolidBrush(RGB(255, 215, 0));
    HPEN coinPen = CreatePen(PS_SOLID, 1, RGB(200, 170, 0));

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, coinBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, coinPen);

    Ellipse(hdc, x - 4, y - 4, x + 4, y + 4);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(coinBrush);
    DeleteObject(coinPen);
}