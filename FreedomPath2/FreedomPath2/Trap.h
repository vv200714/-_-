#pragma once
#pragma once

// Предварительное объявление Game
class Game;

class Trap {
public:
    Trap();
    void initialize(Game* game);
    void render();
    void deactivate() { active = false; }

    // Setters
    void setPosition(float x, float y);

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
    int damage;
};