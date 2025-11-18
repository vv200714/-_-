#pragma once
#pragma once

class Game;

class Wall {
public:
    Wall();
    void initialize(Game* game);
    void render();

    // Setters
    void setPosition(float x, float y);
    void setSize(float width, float height);

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool isSolid() const { return solid; }

private:
    Game* game;
    float x, y;
    float width, height;
    bool solid;
};