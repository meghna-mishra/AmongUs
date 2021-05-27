#include "main.h"

#ifndef HEAD
#define HEAD_H


class Head {
public:
    Head() {}
    Head(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // HEAD_H
