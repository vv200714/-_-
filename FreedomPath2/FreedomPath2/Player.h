#pragma once
#pragma once
#include <windows.h>

// Предварительное объявление Game
class Game;

class Player {
public:
    Player();
    void initialize(Game* game);
    void update();
    void render();
    void handleInput();
    void checkCollisions();
    void reset();

    // Setters
    void setPosition(float x, float y);
    void takeDamage(int damage);
    void addCoin();

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    int getHealth() const { return health; }
    bool hasKey() const { return hasKeyItem; }
    void giveKey() { hasKeyItem = true; }

private:
    Game* game;
    float x, y;
    float width, height;
    int health;
    int maxHealth;
    bool hasKeyItem;
    int coinsCollected;
    float speed;
};