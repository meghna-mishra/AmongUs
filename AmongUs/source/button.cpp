#include "button.h"
#include "main.h"

Button::Button(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;

    int index = 0;
    float x_shift = 0.25;
    float y_shift = -0.25;
    float radius = 0.1;
    
    static GLfloat vertex_buffer_data[2000];
    float i;
    float x1, x2, y1, y2;
    int j;

    for (i = 0; i <= 6.28; i += 0.1)
    {
        x1 = sin(i) * radius + x_shift;
        x2 = sin(i + 0.1) * radius + x_shift;
        y1 = cos(i) * radius + y_shift;
        y2 = cos(i + 0.1) * radius + y_shift;
        float arr[] = {x1, y1, 0, x2, y2, 0, x_shift, y_shift, 0};
        for (j = 0; j <= 8; j++)
        {
            vertex_buffer_data[index] = arr[j];
            index++;
        }
    }
    int nov = index/3;
    this->object = create3DObject(GL_TRIANGLES, nov, vertex_buffer_data, color, GL_FILL);

}

void Button::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Button::set_position(float x, float y) {
   this->position = glm::vec3(x, y, 0);
}

void Button::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

