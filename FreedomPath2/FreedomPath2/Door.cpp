#include "Door.h"
#include "Game.h"

Door::Door() :
    game(nullptr), x(0), y(0), width(16), height(32),
    opened(false), keyRequired(false) {
}

void Door::initialize(Game* gameInstance, bool requiresKey) {
    game = gameInstance;
    keyRequired = requiresKey;
    opened = false;
}

void Door::render() {
    if (game && !opened) {
        game->getTextureManager().drawDoor(game->getMemoryDC(),
            static_cast<int>(x),
            static_cast<int>(y),
            keyRequired);
    }
}

void Door::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}