#include "Player.h"
#include "Game.h"
#include "Zombie.h"
#include "Door.h"
#include "Trap.h"
#include "Coin.h"
#include <algorithm>  // Добавляем для std::max
#include <cmath>

Player::Player() :
    game(nullptr), x(0), y(0), width(16), height(16),
    health(100), maxHealth(100), hasKeyItem(false),
    coinsCollected(0), speed(3.0f) {
}

void Player::initialize(Game* gameInstance) {
    game = gameInstance;
    reset();
}

void Player::reset() {
    health = maxHealth;
    hasKeyItem = false;
    coinsCollected = 0;
    x = 400.0f;
    y = 300.0f;
}

void Player::update() {
    // Обновление состояния игрока
}

void Player::render() {
    if (game) {
        game->getTextureManager().drawPlayer(game->getMemoryDC(),
            static_cast<int>(x),
            static_cast<int>(y));
    }
}

void Player::handleInput() {
    if (!game) return;

    float newX = x;
    float newY = y;

    if (GetAsyncKeyState('W') & 0x8000) newY -= speed;
    if (GetAsyncKeyState('S') & 0x8000) newY += speed;
    if (GetAsyncKeyState('A') & 0x8000) newX -= speed;
    if (GetAsyncKeyState('D') & 0x8000) newX += speed;

    // Проверка границ
    if (newX >= 0 && newX <= game->getScreenWidth() - width) {
        x = newX;
    }
    if (newY >= 0 && newY <= game->getScreenHeight() - height) {
        y = newY;
    }
}

void Player::checkCollisions() {
    if (!game) return;

    // Столкновения с зомби
    auto& zombies = game->getZombies();
    for (auto& zombie : zombies) {
        if (zombie.isActive()) {
            float zombieX = zombie.getX();
            float zombieY = zombie.getY();
            float zombieWidth = zombie.getWidth();
            float zombieHeight = zombie.getHeight();

            if (x < zombieX + zombieWidth &&
                x + width > zombieX &&
                y < zombieY + zombieHeight &&
                y + height > zombieY) {
                takeDamage(10);
            }
        }
    }

    // Столкновения с ловушками
    auto& traps = game->getTraps();
    for (auto& trap : traps) {
        if (trap.isActive()) {
            float trapX = trap.getX();
            float trapY = trap.getY();
            float trapWidth = trap.getWidth();
            float trapHeight = trap.getHeight();

            if (x < trapX + trapWidth &&
                x + width > trapX &&
                y < trapY + trapHeight &&
                y + height > trapY) {
                takeDamage(15);
                trap.deactivate();
            }
        }
    }

    // Столкновения с дверями
    auto& doors = game->getDoors();
    for (auto& door : doors) {
        if (!door.isOpen()) {
            float doorX = door.getX();
            float doorY = door.getY();
            float doorWidth = door.getWidth();
            float doorHeight = door.getHeight();

            if (x < doorX + doorWidth &&
                x + width > doorX &&
                y < doorY + doorHeight &&
                y + height > doorY) {

                if (door.requiresKey() && hasKeyItem) {
                    door.open();
                    hasKeyItem = false;
                    game->addKey(); // Для статистики
                }
                else if (!door.requiresKey()) {
                    door.open();
                }
            }
        }
    }

    // Столкновения с монетами
    auto& coins = game->getCoins();
    for (auto it = coins.begin(); it != coins.end();) {
        if (!it->isCollected()) {
            float coinX = it->getX();
            float coinY = it->getY();
            float coinRadius = it->getRadius();

            float dx = x - coinX;
            float dy = y - coinY;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < width / 2 + coinRadius) {
                it->collect();
                addCoin();
                it = coins.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void Player::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Player::takeDamage(int damage) {
    health = (std::max)(0, health - damage);
}

void Player::addCoin() {
    coinsCollected++;
    if (game) {
        game->addScore(10);
    }
}