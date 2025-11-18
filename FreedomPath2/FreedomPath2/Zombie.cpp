#include "Zombie.h"
#include "Game.h"
#include "Player.h"
#include <algorithm>
#include <cmath>
#include <random>

Zombie::Zombie() :
    game(nullptr), x(0), y(0), width(16), height(16),
    active(true), speed(1.0f), health(50) {
}

void Zombie::initialize(Game* gameInstance) {
    game = gameInstance;
    active = true;
}

void Zombie::update() {
    if (!game || !active) return;

    chasePlayer();
}

void Zombie::render() {
    if (game && active) {
        game->getTextureManager().drawZombie(game->getMemoryDC(),
            static_cast<int>(x),
            static_cast<int>(y));
    }
}

void Zombie::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Zombie::chasePlayer() {
    if (!game) return;

    Player& player = game->getPlayer();
    float playerX = player.getX();
    float playerY = player.getY();

    // Вектор направления к игроку
    float dx = playerX - x;
    float dy = playerY - y;

    // Нормализация
    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0) {
        dx /= length;
        dy /= length;

        // Движение к игроку
        x += dx * speed;
        y += dy * speed;
    }
}