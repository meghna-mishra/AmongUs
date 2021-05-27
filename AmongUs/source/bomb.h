#include "main.h"

#ifndef BOMB
#define BOMB_H


class Bomb {
public:
    Bomb() {}
    Bomb(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // BOMB_H
