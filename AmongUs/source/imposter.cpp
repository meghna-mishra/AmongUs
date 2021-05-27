#include "imposter.h"
#include "main.h"

Imposter::Imposter(float x, float y, color_t color) {

    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;

    static const GLfloat vertex_buffer_data[4][18] = {
        {0.15, -0.15, 0,
        0.35, -0.15, 0,
        0.35, -0.35, 0,
        0.15, -0.15, 0,
        0.15, -0.35, 0,
        0.35, -0.35, 0},
        {0.22, -0.35, 0,
        0.18, -0.35, 0,
        0.22, -0.38, 0,
        0.22, -0.38, 0,
        0.18, -0.35, 0,
        0.18, -0.38, 0},
        {0.28, -0.35, 0,
        0.32, -0.35, 0,
        0.28, -0.38, 0,
        0.28, -0.38, 0,
        0.32, -0.35, 0,
        0.32, -0.38, 0},
        {0.20, -0.12, 0,
        0.30, -0.12, 0,
        0.20, -0.18, 0,
        0.30, -0.12, 0,
        0.20, -0.18, 0,
        0.30, -0.18, 0}
    };

    color_t colarr[] = {COLOR_RED, COLOR_RED, COLOR_RED, COLOR_BLACK};
    for(int i = 0; i < 4; i++){
        //std::cout << i << "\n";
        this->object[i] = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data[i], colarr[i], GL_FILL);
    }
}

void Imposter::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    for(int i=0; i<4; i++){
        draw3DObject(this->object[i]);
    }
}

void Imposter::set_position(float x, float y) {
   this->position = glm::vec3(x, y, 0);
}

void Imposter::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

