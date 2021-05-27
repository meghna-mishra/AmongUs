#include "main.h"

#ifndef IMPOSTER_H
#define IMPOSTER_H


class Imposter {
public:
    Imposter() {}
    Imposter(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object[4];
};

#endif // IMPOSTER_H
