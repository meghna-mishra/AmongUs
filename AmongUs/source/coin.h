#include "main.h"

#ifndef COIN
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // COIN_H
