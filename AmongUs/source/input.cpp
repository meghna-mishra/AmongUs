#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"

bool   cannon_keyboard_input = true;
bool   drag_pan = false, old_cki;
double drag_oldx = -1, drag_oldy = -1;

using namespace std;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        // switch (key) {
        // case GLFW_KEY_C:
        // rectangle_rot_status = !rectangle_rot_status;
        // break;
        // case GLFW_KEY_P:
        // triangle_rot_status = !triangle_rot_status;
        // break;
        // case GLFW_KEY_X:
        //// do something ..
        // break;
        // default:
        // break;
        // }
    } else if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            quit(window);
            break;
        default:
            break;
        }
    }
}

/* Executed for character input (like in text boxes) */
//HANDLING COLLISIONS WITH WALLS
void keyboardChar(GLFWwindow *window, unsigned int key) {
    int i, j;
    int move = 1;
    switch (key) {
    case 'Q':
    case 'q':
        quit(window);
        break;
    case 'a':
        for(i = 0; i < 5; i++){
            for(j = 0; j < 5; j++){
                if(i == 0 && j == 0){
                    continue;
                }
                else if(mazearr[i][j] == 'U' && player_x == (0.5*j) && player_y == (-0.5*i)){
                    move = 0;
                    break;
                }
                else if (player_x == 0){
                    move = 0;
                    break;
                }
                else{
                    move = 1;
                }
            }
            if(move == 0){
                    break;
                }
        }
        if(move == 1){
            player_x -= 0.5;
        }
        break;
    case 'd':
        for(i = 0; i < 5; i++){
            for(j = 0; j < 5; j++){
                if(i == 0 && j == 0){
                    continue;
                }
                else if(mazearr[i][j+1] == 'U' && player_x+0.5 == (0.5*(j+1)) && player_y == (-0.5*i)){
                    move = 0;
                    break;
                }
                else if(player_x == 2.0){
                    move = 0;
                    break;
                }
                else{
                    move = 1;
                }
            }
            if(move == 0){
                    break;
                }
        }
        if(move == 1){
            player_x += 0.5;
        }
        break;
    case 'w':
         for(i = 0; i < 5; i++){
            for(j = 0; j < 5; j++){
                if(i == 0 && j == 0){
                    continue;
                }
                else if(mazearr[i][j] == 'L' && player_x == (0.5*j) && player_y == (-0.5*i)){
                    move = 0;
                    break;
                }
                else if(player_y == 0){
                    move = 0;
                    break;
                }
                else{
                    move = 1;
                }
            }
            if(move == 0){
                    break;
                }
        }
        if(move == 1){
            player_y += 0.5;
        }
         break;
    case 's':
        for(i = 0; i < 5; i++){
            for(j = 0; j < 5; j++){
                if(i == 0 && j == 0){
                    continue;
                }
                else if(mazearr[i+1][j] == 'L' && player_x == (0.5*j) && player_y-0.5 == (-0.5*(i+1))){
                    move = 0;
                    break;
                }
                else if(player_y == -2.0){
                    move = 0;
                    break;
                }
                else{
                    move = 1;
                }
            }
            if(move == 0){
                    break;
                }
        }
        if(move == 1){
            player_y -= 0.5;
        }
        break;
    default:
        break;
    }
}

/* Executed when a mouse button is pressed/released */
void mouseButton(GLFWwindow *window, int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS) {
            // Do something
            return;
        } else if (action == GLFW_RELEASE) {
            // Do something
        }
        break;
    // case GLFW_MOUSE_BUTTON_RIGHT:
    // if (action == GLFW_RELEASE) {
    // rectangle_rot_dir *= -1;
    // }
    // break;
    default:
        break;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // Do something
}
