#include "Game.h"
#include "Player.h"
#include "Zombie.h"
#include "Door.h"
#include "Trap.h"
#include "Coin.h"
#include <sstream>
#include <chrono>
#include <random>

Game::Game() :
    hwnd(nullptr), hdc(nullptr), memDC(nullptr), hBitmap(nullptr),
    screenWidth(800), screenHeight(600),
    currentState(GameState::MENU),
    gameFont(nullptr), titleFont(nullptr), russianFont(nullptr),  // Добавлена инициализация шрифтов
    player(nullptr),
    score(0), keysCollected(0),
    lastUpdateTime(GetTickCount64()) {

    player = new Player();
}

Game::~Game() {
    cleanupFonts();  // Добавлен вызов очистки шрифтов
    if (player) delete player;
    if (hBitmap) DeleteObject(hBitmap);
    if (memDC) DeleteDC(memDC);
    if (hdc) ReleaseDC(hwnd, hdc);
}

bool Game::initialize() {
    initializeWindow();
    textureManager.initialize();
    initializeFonts();  // Добавлен вызов инициализации шрифтов
    initializeMenu();
    initializeGameObjects();
    return true;
}

// Метод для создания шрифтов с поддержкой русского
void Game::initializeFonts() {
    cleanupFonts();

    // Шрифт для русского текста
    russianFont = CreateFont(
        24,                           // Высота
        0,                            // Ширина
        0,                            // Угол наклона
        0,                            // Угол ориентации
        FW_NORMAL,                    // Толщина
        FALSE,                        // Курсив
        FALSE,                        // Подчеркивание
        FALSE,                        // Зачеркивание
        RUSSIAN_CHARSET,              // РУССКАЯ КОДИРОВКА!
        OUT_DEFAULT_PRECIS,           // Точность вывода
        CLIP_DEFAULT_PRECIS,          // Точность отсечения
        DEFAULT_QUALITY,              // Качество
        DEFAULT_PITCH | FF_DONTCARE,  // Ширина и семейство
        L"Times New Roman"            // Шрифт поддерживающий кириллицу
    );

    // Жирный шрифт для заголовков
    titleFont = CreateFont(
        36,                           // Высота
        0,                            // Ширина
        0,                            // Угол наклона
        0,                            // Угол ориентации
        FW_BOLD,                      // Жирный
        FALSE,                        // Курсив
        FALSE,                        // Подчеркивание
        FALSE,                        // Зачеркивание
        RUSSIAN_CHARSET,              // РУССКАЯ КОДИРОВКА
        OUT_DEFAULT_PRECIS,           // Точность вывода
        CLIP_DEFAULT_PRECIS,          // Точность отсечения
        DEFAULT_QUALITY,              // Качество
        DEFAULT_PITCH | FF_DONTCARE,  // Ширина и семейство
        L"Arial"                      // Шрифт поддерживающий кириллицу
    );

    // Обычный шрифт для текста
    gameFont = CreateFont(
        20,                           // Высота
        0,                            // Ширина
        0,                            // Угол наклона
        0,                            // Угол ориентации
        FW_NORMAL,                    // Толщина
        FALSE,                        // Курсив
        FALSE,                        // Подчеркивание
        FALSE,                        // Зачеркивание
        RUSSIAN_CHARSET,              // РУССКАЯ КОДИРОВКА
        OUT_DEFAULT_PRECIS,           // Точность вывода
        CLIP_DEFAULT_PRECIS,          // Точность отсечения
        DEFAULT_QUALITY,              // Качество
        DEFAULT_PITCH | FF_DONTCARE,  // Ширина и семейство
        L"Verdana"                    // Шрифт поддерживающий кириллицу
    );
}

// Метод для рисования русского текста
void Game::drawRussianText(int x, int y, const std::wstring& text, COLORREF color, bool isTitle) {
    // Выбираем шрифт
    HFONT oldFont = (HFONT)SelectObject(memDC, isTitle ? titleFont : gameFont);

    SetTextColor(memDC, color);
    SetBkMode(memDC, TRANSPARENT);

    // Рисуем текст (уже в wide-string формате)
    TextOut(memDC, x, y, text.c_str(), static_cast<int>(text.length()));

    // Восстанавливаем старый шрифт
    SelectObject(memDC, oldFont);
}

// Метод для очистки шрифтов
void Game::cleanupFonts() {
    if (gameFont) DeleteObject(gameFont);
    if (titleFont) DeleteObject(titleFont);
    if (russianFont) DeleteObject(russianFont);
    gameFont = titleFont = russianFont = nullptr;
}

// Остальной код без изменений...
void Game::initializeWindow() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"FreedomPath";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, L"FreedomPath", L"Путь к свободе",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        screenWidth, screenHeight, NULL, NULL,
        GetModuleHandle(NULL), this);

    hdc = GetDC(hwnd);
    memDC = CreateCompatibleDC(hdc);
    hBitmap = CreateCompatibleBitmap(hdc, screenWidth, screenHeight);
    SelectObject(memDC, hBitmap);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}

void Game::initializeMenu() {
    menuButtons = {
        {{300, 200, 500, 250}, L"Новая игра", false},     // Кнопка 1
        {{300, 270, 500, 320}, L"Загрузить игру", false}, // Кнопка 2  
        {{300, 340, 500, 390}, L"Настройки", false},      // Кнопка 3
        {{300, 410, 500, 460}, L"Выход", false}           // Кнопка 4
    };
}

void Game::initializeGameObjects() {
    player->initialize(this);
    player->setPosition(400.0f, 300.0f);
    spawnEntities();
}

void Game::spawnEntities() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> posDist(50, 750);

    // Зомби
    zombies.clear();
    for (int i = 0; i < 5; ++i) {
        Zombie zombie;
        zombie.initialize(this);
        zombie.setPosition(static_cast<float>(posDist(gen)),
            static_cast<float>(posDist(gen)));
        zombies.push_back(zombie);
    }

    // Двери
    doors.clear();
    Door exitDoor;
    exitDoor.initialize(this, false);
    exitDoor.setPosition(700.0f, 500.0f);
    doors.push_back(exitDoor);

    Door keyDoor;
    keyDoor.initialize(this, true);
    keyDoor.setPosition(100.0f, 100.0f);
    doors.push_back(keyDoor);

    // Ловушки
    traps.clear();
    for (int i = 0; i < 8; ++i) {
        Trap trap;
        trap.initialize(this);
        trap.setPosition(static_cast<float>(posDist(gen)),
            static_cast<float>(posDist(gen)));
        traps.push_back(trap);
    }

    // Монеты
    coins.clear();
    for (int i = 0; i < 15; ++i) {
        Coin coin;
        coin.initialize(this);
        coin.setPosition(static_cast<float>(posDist(gen)),
            static_cast<float>(posDist(gen)));
        coins.push_back(coin);
    }
}

void Game::run() {
    MSG msg = {};
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            DWORD currentTime = GetTickCount64();
            float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
            lastUpdateTime = currentTime;

            handleInput();
            if (currentState == GameState::PLAYING) {
                update();
            }
            render();
            Sleep(16);
        }
    }
}

void Game::update() {
    player->update();

    for (auto& zombie : zombies) {
        zombie.update();
    }

    checkCollisions();

    if (player->getHealth() <= 0) {
        currentState = GameState::GAME_OVER;
    }
}

void Game::checkCollisions() {
    player->checkCollisions();
}

void Game::handleInput() {
    if (currentState == GameState::PLAYING) {
        player->handleInput();
    }
}

void Game::render() {
    // Очистка экрана
    RECT clearRect = { 0, 0, screenWidth, screenHeight };
    HBRUSH bgBrush = CreateSolidBrush(RGB(30, 20, 10));
    FillRect(memDC, &clearRect, bgBrush);
    DeleteObject(bgBrush);

    switch (currentState) {
    case GameState::MENU: renderMenu(); break;
    case GameState::PLAYING: renderGame(); break;
    case GameState::GAME_OVER: renderGame(); renderGameOver(); break;
    case GameState::WIN: renderGame(); renderWin(); break;
    }

    BitBlt(hdc, 0, 0, screenWidth, screenHeight, memDC, 0, 0, SRCCOPY);
}

void Game::renderMenu() {
    // Фон меню
    textureManager.drawMenuBackground(memDC);

    // Заголовок - теперь используем русский шрифт
    drawRussianText(250, 100, L"ПУТЬ К СВОБОДЕ", RGB(255, 0, 0), true);

    // Кнопки
    for (const auto& button : menuButtons) {
        // Отрисовка прямоугольника кнопки
        COLORREF btnColor = button.hovered ? RGB(120, 90, 70) : RGB(100, 70, 50);
        HBRUSH brush = CreateSolidBrush(btnColor);
        RECT rect = button.rect;
        FillRect(memDC, &rect, brush);
        DeleteObject(brush);
        // Рамка
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(70, 50, 30));
        HPEN oldPen = (HPEN)SelectObject(memDC, pen);
        MoveToEx(memDC, rect.left, rect.top, NULL);
        LineTo(memDC, rect.right, rect.top);
        LineTo(memDC, rect.right, rect.bottom);
        LineTo(memDC, rect.left, rect.bottom);
        LineTo(memDC, rect.left, rect.top);
        SelectObject(memDC, oldPen);
        DeleteObject(pen);

        // Текст кнопки - теперь используем русский шрифт
        std::wstring wtext(button.text.begin(), button.text.end());
        drawRussianText(button.rect.left + 20, button.rect.top + 15,
            wtext, RGB(255, 255, 255), false);
    }
}

void Game::renderGame() {
    // Фон
    textureManager.drawGameBackground(memDC);

    // Объекты
    for (auto& trap : traps) {
        trap.render();
    }
    for (auto& door : doors) {
        door.render();
    }
    for (auto& coin : coins) {
        coin.render();
    }
    for (auto& zombie : zombies) {
        zombie.render();
    }
    player->render();

    // UI - теперь используем русский шрифт
    std::wstringstream wss;
    wss << L"Монеты: " << score;
    drawRussianText(10, 10, wss.str(), RGB(255, 255, 255), false);

    wss.str(L"");
    wss << L"Здоровье: " << player->getHealth();
    drawRussianText(10, 30, wss.str(), RGB(255, 100, 100), false);

    wss.str(L"");
    wss << L"Ключи: " << keysCollected;
    drawRussianText(10, 50, wss.str(), RGB(255, 255, 0), false);
}

void Game::renderGameOver() {
    HBRUSH overlay = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = { 0, 0, screenWidth, screenHeight };
    FillRect(memDC, &rect, overlay);
    DeleteObject(overlay);

    drawRussianText(200, 250, L"ИГРА ОКОНЧЕНА!", RGB(255, 0, 0), true);
    drawRussianText(250, 280, L"Нажмите ESC для выхода в меню", RGB(255, 255, 255), false);
}

void Game::renderWin() {
    HBRUSH overlay = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = { 0, 0, screenWidth, screenHeight };
    FillRect(memDC, &rect, overlay);
    DeleteObject(overlay);

    drawRussianText(250, 250, L"ПОБЕДА!", RGB(0, 255, 0), true);
    drawRussianText(200, 280, L"Вы нашли путь к свободе!", RGB(255, 255, 255), false);
}

void Game::drawText(int x, int y, const std::string& text, COLORREF color) {
    SetTextColor(memDC, color);
    SetBkMode(memDC, TRANSPARENT);

    std::wstring wtext(text.begin(), text.end());
    TextOut(memDC, x, y, wtext.c_str(), static_cast<int>(wtext.length()));
}

void Game::resetGame() {
    score = 0;
    keysCollected = 0;
    player->reset();
    initializeGameObjects();
    currentState = GameState::PLAYING;
}

Player& Game::getPlayer() {
    return *player;
}

void Game::handleMenuInput() {
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(hwnd, &mousePos);

    for (auto& button : menuButtons) {
        button.hovered = (mousePos.x >= button.rect.left &&
            mousePos.x <= button.rect.right &&
            mousePos.y >= button.rect.top &&
            mousePos.y <= button.rect.bottom);
    }
}

LRESULT CALLBACK Game::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Game* game = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (uMsg == WM_CREATE) {
        CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
        game = reinterpret_cast<Game*>(create->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(game));
    }

    if (game) {
        switch (uMsg) {
        case WM_MOUSEMOVE:
            game->handleMenuInput();
            break;

        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            if (game->currentState == GameState::MENU) {
                for (size_t i = 0; i < game->menuButtons.size(); ++i) {
                    const auto& button = game->menuButtons[i];
                    if (x >= button.rect.left && x <= button.rect.right &&
                        y >= button.rect.top && y <= button.rect.bottom) {

                        switch (i) {
                        case 0: game->resetGame(); break;
                        case 3: PostQuitMessage(0); break;
                        }
                    }
                }
            }
            break;
        }

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                if (game->currentState == GameState::PLAYING ||
                    game->currentState == GameState::GAME_OVER ||
                    game->currentState == GameState::WIN) {
                    game->currentState = GameState::MENU;
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}