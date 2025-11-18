#pragma once

class Game;

class Door {
public:
    Door();
    void initialize(Game* game, bool requiresKey);
    void render();
    void open() { opened = true; }  // Изменено имя поля

    // Setters
    void setPosition(float x, float y);

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool isOpen() const { return opened; }      // Метод остается
    bool requiresKey() const { return keyRequired; }

private:
    Game* game;
    float x, y;
    float width, height;
    bool opened;           // Изменено имя поля
    bool keyRequired;      // Изменено имя поля
};