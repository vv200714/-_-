#include "Coin.h"
#include "Game.h"

Coin::Coin() :
    game(nullptr), x(0), y(0), radius(4), collected(false) {
}

void Coin::initialize(Game* gameInstance) {
    game = gameInstance;
    collected = false;
}

void Coin::render() {
    if (game && !collected) {
        game->getTextureManager().drawCoin(game->getMemoryDC(),
            static_cast<int>(x),
            static_cast<int>(y));
    }
}

void Coin::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}