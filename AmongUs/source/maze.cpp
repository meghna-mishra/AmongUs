#include "maze.h"
#include "main.h"

Maze::Maze(float x, float y, color_t color) {

    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;

    int i, j;
    srand(time(NULL));
    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            char temp[2];
            int var = 0;
            if(i > 0){
                temp[var] = 'U';
                var++;
            }
            if(j > 0){
                temp[var] = 'L';
                var++;
            }
            if(var==0){
                continue;
            }
            int r = rand()%2;
            if(var==2){
                mazearr[i][j] = temp[r];
            }
            else{
                mazearr[i][j] = temp[0];
            }
        }
    }

    static GLfloat vertex_buffer_data[5000];

    int k = 0;
    for(i = 0; i < 5; i++){
    	for(j = 0; j < 5; j++){
    		if((i == 0) && (j == 0)){
    			continue;
    		}
    		if(mazearr[i][j] == 'U'){
    			vertex_buffer_data[k++] = j*0.5;
    			vertex_buffer_data[k++] = -(i*0.5);
    			vertex_buffer_data[k++] = 0;
    			vertex_buffer_data[k++] = j*0.5;
    			vertex_buffer_data[k++] = -(i*0.5 + 0.5);
    			vertex_buffer_data[k++] = 0;
    			//std::cout << mazearr[i][j] << " ";
    		}
    		if(mazearr[i][j] == 'L'){
    			vertex_buffer_data[k++] = j*0.5;
    			vertex_buffer_data[k++] = -(i*0.5);
    			vertex_buffer_data[k++] = 0;
    			vertex_buffer_data[k++] = j*0.5 + 0.5;
    			vertex_buffer_data[k++] = -(i*0.5);
    			vertex_buffer_data[k++] = 0;
    			//std::cout << mazearr[i][j] << " ";
    		}
    	}
    	//std::cout << "\n";
    }

    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = -0.5;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0.5;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0;

    vertex_buffer_data[k++] = 2.5;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 2.5;
    vertex_buffer_data[k++] = -2.5;
    vertex_buffer_data[k++] = 0;

    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = -2.5;
    vertex_buffer_data[k++] = 0;
    vertex_buffer_data[k++] = 2.5;
    vertex_buffer_data[k++] = -2.5;
    vertex_buffer_data[k++] = 0;
    this->object = create3DObject(GL_LINES, 28*2, vertex_buffer_data, color, GL_FILL);
}

void Maze::draw(glm::mat4 VP) {
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

void Maze::set_position(float x, float y) {
   this->position = glm::vec3(x, y, 0);
}

void Maze::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

