#include "Trap.h"
#include "Game.h"

Trap::Trap() :
    game(nullptr), x(0), y(0), width(16), height(16),
    active(true), damage(15) {
}

void Trap::initialize(Game* gameInstance) {
    game = gameInstance;
    active = true;
}

void Trap::render() {
    if (game && active) {
        game->getTextureManager().drawTrap(game->getMemoryDC(),
            static_cast<int>(x),
            static_cast<int>(y));
    }
}

void Trap::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}