#pragma once
#pragma once

// Предварительное объявление Game
class Game;

class Coin {
public:
    Coin();
    void initialize(Game* game);
    void render();
    void collect() { collected = true; }

    // Setters
    void setPosition(float x, float y);

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }
    bool isCollected() const { return collected; }

private:
    Game* game;
    float x, y;
    float radius;
    bool collected;
};