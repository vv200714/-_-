#pragma once
#pragma once

// Предварительное объявление Game
class Game;

class Zombie {
public:
    Zombie();
    void initialize(Game* game);
    void update();
    void render();

    // Setters
    void setPosition(float x, float y);
    void deactivate() { active = false; }

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool isActive() const { return active; }

private:
    Game* game;
    float x, y;
    float width, height;
    bool active;
    float speed;
    int health;

    void chasePlayer();
};