#include "head.h"
#include "main.h"

Head::Head(float x, float y, color_t color) {

    int vertex_buffer_data_index = 0;
    float x_init = 0.25;
    float y_init = -0.15;
    float radius = 0.1;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    
    static GLfloat vertex_buffer_data[2000];

    for (float i = 0; i <= 6.28; i += 0.1)
    {
        float x1 = sin(i) * radius + x_init;
        float x2 = sin(i + 0.1) * radius + x_init;
        float y1 = cos(i) * radius + y_init;
        float y2 = cos(i + 0.1) * radius + y_init;
        float arr[] = {x1, y1, 0, x2, y2, 0, x_init, y_init, 0};
        for (int j = 0; j < 9; j++)
        {
            vertex_buffer_data[vertex_buffer_data_index] = arr[j];
            vertex_buffer_data_index++;
        }
    }
    this->object = create3DObject(GL_TRIANGLES, vertex_buffer_data_index/3, vertex_buffer_data, color, GL_FILL);

    //this->object = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data, color, GL_FILL);
}

void Head::draw(glm::mat4 VP) {
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

void Head::set_position(float x, float y) {
   this->position = glm::vec3(x, y, 0);
}

void Head::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

