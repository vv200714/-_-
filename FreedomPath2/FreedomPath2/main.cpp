#include "Game.h"

// Аннотации для WinMain
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
    // Подавляем неиспользуемые параметры
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nShowCmd;

    Game game;
    if (game.initialize()) {
        game.run();
    }
    return 0;
}