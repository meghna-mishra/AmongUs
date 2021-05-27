#include "main.h"
#include "timer.h"
#include "maze.h"
#include "player.h"
#include "imposter.h"
#include "button.h"
#include "coin.h"
#include "bomb.h"
#include "head.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <bits/stdc++.h>

#define GLT_IMPLEMENTATION
#include "gltext.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Maze maze;
Player player;
Imposter imposter;
Button button;
Button button2;
Coin coin1, coin2;
Bomb bomb1;
Head head1, head2;

char mazearr[5][5];
int vaporise = 0;
bool gameover = false;
int health = 5;
int gametime = 60;
bool activated = false;

vector<pair<pair <int, int>, pair<int, int>>> vec; // vector of pair of pairs storing possible movements in maze

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float player_x = 0, player_y = 0;
float imposter_x = 2.0, imposter_y = -1.0;

bool coin1val = true, coin2val = true;
bool bomb1val = true;
int tasks = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    //glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    //glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    //glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    //Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
     Matrices.view = glm::lookAt(glm::vec3(1.25, -1.25, 3), glm::vec3(1.25, -1.25, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    if(gameover == false){
    button.draw(VP);
    button2.draw(VP);
    maze.draw(VP);
    head1.draw(VP);
    player.draw(VP);
    if(vaporise == 0){
        head2.draw(VP);
        imposter.draw(VP);
    }
    if(activated){
        if(coin1val){
            coin1.draw(VP);
        }
        if(coin2val){
            coin2.draw(VP);
        }
        if(bomb1val){
            bomb1.draw(VP);
        }
    }
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up  = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    }

void tick_elements() {
    maze.tick();
    camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    maze       = Maze(0, 0, COLOR_BLACK);
    player     = Player(player_x, player_y, COLOR_GREEN);
    imposter   = Imposter(2.0, -1.0, COLOR_RED);
    button     = Button(0.5, -2.0, COLOR_RED);
    button2    = Button(2.0, -0.5, COLOR_RED);
    coin1      = Coin(1.0, -1.0, COLOR_YELLOW);
    coin2      = Coin(2.0, -1.5, COLOR_YELLOW);
    bomb1      = Bomb(1.0, -2.0, COLOR_BLACK);
    head1      = Head(player_x, player_y, COLOR_GREEN);
    head2      = Head(2.0, -1.0, COLOR_RED);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

/*void imposter_move(){
    bool goleft = false, goright = false, goup = false, godown = false;
    if(player.position.x == imposter.position.x && player.position.y == imposter.position.y){
        return;
    }
    if(player.position.y > imposter.position.y){
        goup = true;
    }
    if(player.position.y < imposter.position.y){
        godown = true;
    }
    if(player.position.x < imposter.position.x){
        goleft = true;
    }
    if(player.position.x > imposter.position.x){
        goright = true;
    }
    int move = 1;
    if(goleft){
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                if(i == 0 && j == 0){
                    continue;
                }
                else if(mazearr[i][j] == 'U' && imposter.position.x == (0.5*j) && imposter.position.y == (-0.5*i)){
                    move = 0;
                    break;
                }
                else if (imposter.position.x == 0){
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
            imposter.position.x -= 0.5;
        }
        else if(goup){
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                    if(i == 0 && j == 0){
                        continue;
                    }
                    else if(mazearr[i][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y== (-0.5*i)){
                        move = 0;
                        break;
                    }
                    else if(imposter.position.y == 0){
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
                imposter.position.y += 0.5;
            }
            else{
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                        continue;
                        }
                        else if(mazearr[i+1][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y-0.5 == (-0.5*(i+1))){
                            move = 0;
                            break;
                        }
                        else if(imposter.position.y == -2.0){
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
                    imposter.position.y -= 0.5;
                }
                else{
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 5; j++){
                            if(i == 0 && j == 0){
                                continue;
                            }
                            else if(mazearr[i][j+1] == 'U' && imposter.position.x+0.5 == (0.5*(j+1)) && imposter.position.y == (-0.5*i)){
                                move = 0;
                                break;
                            }
                            else if(imposter.position.x == 2.0){
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
                        imposter.position.x += 0.5;
                    }
                }
            }
        }
        else{
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                    if(i == 0 && j == 0){
                        continue;
                    }
                    else if(mazearr[i+1][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y-0.5 == (-0.5*(i+1))){
                        move = 0;
                        break;
                    }
                    else if(imposter.position.y == -2.0){
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
                imposter.position.y -= 0.5;
            }
            else{
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                            continue;
                        }
                        else if(mazearr[i][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y== (-0.5*i)){
                            move = 0;
                            break;
                        }
                        else if(imposter.position.y == 0){
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
                    imposter.position.y += 0.5;
                }
                else{
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 5; j++){
                            if(i == 0 && j == 0){
                                continue;
                            }
                            else if(mazearr[i][j+1] == 'U' && imposter.position.x+0.5 == (0.5*(j+1)) && imposter.position.y == (-0.5*i)){
                                move = 0;
                                break;
                            }
                            else if(imposter.position.x == 2.0){
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
                        imposter.position.x += 0.5;
                    }
                }
            }
        }
    }
    else if(goright){
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                if(i == 0 && j == 0){
                    continue;
                }
                else if(mazearr[i][j+1] == 'U' && imposter.position.x+0.5 == (0.5*(j+1)) && imposter.position.y == (-0.5*i)){
                    move = 0;
                    break;
                }
                else if(imposter.position.x == 2.0){
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
            imposter.position.x += 0.5;
        }
        else if(goup){
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                    if(i == 0 && j == 0){
                        continue;
                    }
                    else if(mazearr[i][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y== (-0.5*i)){
                        move = 0;
                        break;
                    }
                    else if(imposter.position.y == 0){
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
                imposter.position.y += 0.5;
            }
            else{
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                            continue;
                        }
                        else if(mazearr[i+1][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y-0.5 == (-0.5*(i+1))){
                            move = 0;
                            break;
                        }
                        else if(imposter.position.y == -2.0){
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
                    imposter.position.y -= 0.5;
                }
                else{
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 5; j++){
                            if(i == 0 && j == 0){
                                continue;
                            }
                            else if(mazearr[i][j] == 'U' && imposter.position.x == (0.5*j) && imposter.position.y == (-0.5*i)){
                                move = 0;
                                break;
                            }
                            else if (imposter.position.x == 0){
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
                        imposter.position.x -= 0.5;
                    }
                }
            }
        }
        else{
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                    if(i == 0 && j == 0){
                    continue;
                    }
                    else if(mazearr[i+1][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y-0.5 == (-0.5*(i+1))){
                        move = 0;
                        break;
                    }
                    else if(imposter.position.y == -2.0){
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
                imposter.position.y -= 0.5;
            }
            else{//go up then left
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                            continue;
                        }
                        else if(mazearr[i][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y== (-0.5*i)){
                            move = 0;
                            break;
                        }
                        else if(imposter.position.y == 0){
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
                    imposter.position.y += 0.5;
                }
                else{
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 5; j++){
                            if(i == 0 && j == 0){
                                continue;
                            }
                            else if(mazearr[i][j] == 'U' && imposter.position.x == (0.5*j) && imposter.position.y == (-0.5*i)){
                                move = 0;
                                break;
                            }
                            else if (imposter.position.x == 0){
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
                        imposter.position.x -= 0.5;
                    }
                }
            }
        }
    }
    else if(godown){
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                if(i == 0 && j == 0){
                continue;
                }
                else if(mazearr[i+1][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y-0.5 == (-0.5*(i+1))){
                    move = 0;
                    break;
                }
                else if(imposter.position.y == -2.0){
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
            imposter.position.y -= 0.5;
        }
        else if(goright){
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                    if(i == 0 && j == 0){
                        continue;
                    }
                    else if(mazearr[i][j+1] == 'U' && imposter.position.x+0.5 == (0.5*(j+1)) && imposter.position.y == (-0.5*i)){
                        move = 0;
                        break;
                    }
                    else if(imposter.position.x == 2.0){
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
                imposter.position.x += 0.5;
            }
            else{//go left
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                            continue;
                        }
                        else if(mazearr[i][j] == 'U' && imposter.position.x == (0.5*j) && imposter.position.y == (-0.5*i)){
                            move = 0;
                            break;
                        }
                        else if (imposter.position.x == 0){
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
                    imposter.position.x -= 0.5;
                }
                else{//go up
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 5; j++){
                            if(i == 0 && j == 0){
                                continue;
                            }
                            else if(mazearr[i][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y== (-0.5*i)){
                                move = 0;
                                break;
                            }
                            else if(imposter.position.y == 0){
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
                        imposter.position.y += 0.5;
                    }
                }
            }
        }
        else{//go left then right then up
            for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                            continue;
                        }
                        else if(mazearr[i][j] == 'U' && imposter.position.x == (0.5*j) && imposter.position.y == (-0.5*i)){
                            move = 0;
                            break;
                        }
                        else if (imposter.position.x == 0){
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
                    imposter.position.x -= 0.5;
                }
                else{
                    for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                            continue;
                        }
                        else if(mazearr[i][j+1] == 'U' && imposter.position.x+0.5 == (0.5*(j+1)) && imposter.position.y == (-0.5*i)){
                            move = 0;
                            break;
                        }
                        else if(imposter.position.x == 2.0){
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
                    imposter.position.x += 0.5;
                }
                else{
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 5; j++){
                            if(i == 0 && j == 0){
                                continue;
                            }
                            else if(mazearr[i][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y== (-0.5*i)){
                                move = 0;
                                break;
                            }
                            else if(imposter.position.y == 0){
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
                        imposter.position.y += 0.5;
                    }
                }
            }
        }
    }//godown ends
    else if(goup){
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                if(i == 0 && j == 0){
                    continue;
                }
                else if(mazearr[i][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y== (-0.5*i)){
                    move = 0;
                    break;
                }
                else if(imposter.position.y == 0){
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
            imposter.position.y += 0.5;
        }
        else if(goright){
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                    if(i == 0 && j == 0){
                        continue;
                    }
                    else if(mazearr[i][j+1] == 'U' && imposter.position.x+0.5 == (0.5*(j+1)) && imposter.position.y == (-0.5*i)){
                        move = 0;
                        break;
                    }
                    else if(imposter.position.x == 2.0){
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
                imposter.position.x += 0.5;
            }
            else{//go left
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                            continue;
                        }
                        else if(mazearr[i][j] == 'U' && imposter.position.x == (0.5*j) && imposter.position.y == (-0.5*i)){
                            move = 0;
                            break;
                        }
                        else if (imposter.position.x == 0){
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
                    imposter.position.x -= 0.5;
                }
                else{//go down
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 5; j++){
                            if(i == 0 && j == 0){
                            continue;
                            }
                            else if(mazearr[i+1][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y-0.5 == (-0.5*(i+1))){
                                move = 0;
                                break;
                            }
                            else if(imposter.position.y == -2.0){
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
                        imposter.position.y -= 0.5;
                    }       
                }
            }
        }
        else{
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 5; j++){
                    if(i == 0 && j == 0){
                        continue;
                    }
                    else if(mazearr[i][j] == 'U' && imposter.position.x == (0.5*j) && imposter.position.y == (-0.5*i)){
                        move = 0;
                        break;
                    }
                    else if (imposter.position.x == 0){
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
                imposter.position.x -= 0.5;
            }
            else{//go right then down
                for(int i = 0; i < 5; i++){
                    for(int j = 0; j < 5; j++){
                        if(i == 0 && j == 0){
                            continue;
                        }
                        else if(mazearr[i][j+1] == 'U' && imposter.position.x+0.5 == (0.5*(j+1)) && imposter.position.y == (-0.5*i)){
                            move = 0;
                            break;
                        }
                        else if(imposter.position.x == 2.0){
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
                    imposter.position.x += 0.5;
                }
                else{
                    for(int i = 0; i < 5; i++){
                        for(int j = 0; j < 5; j++){
                            if(i == 0 && j == 0){
                            continue;
                            }
                            else if(mazearr[i+1][j] == 'L' && imposter.position.x == (0.5*j) && imposter.position.y-0.5 == (-0.5*(i+1))){
                                move = 0;
                                break;
                            }
                            else if(imposter.position.y == -2.0){
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
                        imposter.position.y -= 0.5;
                    }
                }
            }
        }
    }
}*/

int imposter_move()//bellman ford
{
    int distance[25];
    int parent[25];
    int p_x = player.position.x * 2, p_y = -(player.position.y) * 2;
    int i_x = imposter.position.x * 2, i_y = -(imposter.position.y) * 2;
    int i,j;
    //cout << p_x << " " << p_y << " " << i_x << " " << i_y << endl;
    int len = vec.size();
    for (i = 0; i < 25; i++)
    {
        distance[i] = 10000;
        parent[i] = -1;
    }
    distance[p_x + p_y*5] = 0;
    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < len; j++)
        {
            int cell1_x = vec[j].first.first;
            int cell1_y = vec[j].first.second;
            int cell2_x = vec[j].second.first;
            int cell2_y = vec[j].second.second;
            int cell1_index = cell1_x + cell1_y*5;
            int cell2_index = cell2_x + cell2_y*5;
            if (distance[cell1_index] != 10000 && distance[cell1_index] + 1 < distance[cell2_index])
            {
                distance[cell2_index] = distance[cell1_index] + 1;
                parent[cell2_index] = cell1_index;
                //cout << cell1_index << " " << cell2_index << endl;
            }
        }
    }
    int val = parent[i_x + i_y*5];
    //imposter.position.x = (val/5)/2;
    //imposter.position.y = -(4 - (val%5))/2;
    imposter.position.x = ((float)(val%5))/(float)2;
    imposter.position.y = -((float)(val/5))/(float)2;
    //cout << val << endl;
    //cout << imposter.position.x << " " << imposter.position.y << endl;
}

int main(int argc, char **argv){
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    int frame = 0;

    int win = -1;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(i == 0 && j == 0){
                continue;
            }
            if(mazearr[i][j] == 'L'){
                vec.push_back(make_pair(make_pair(j, i), make_pair(j-1, i)));
                vec.push_back(make_pair(make_pair(j-1, i), make_pair(j, i)));
            }
            if(mazearr[i][j] == 'U'){
                vec.push_back(make_pair(make_pair(j, i), make_pair(j, i-1)));
                vec.push_back(make_pair(make_pair(j, i-1), make_pair(j, i)));
            }
        }
    }

    /*for(int i = 0; i < vec.size(); i++){
        cout << vec[i].first.first << " , ";
        cout << vec[i].first.second << " -> ";
        cout << vec[i].second.first << " , ";
        cout << vec[i].second.second << endl << endl;
    }*/

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            frame++;
            if(frame == 60){
                frame = 0;
                if(vaporise == 0 && gameover == false){
                    imposter_move();
                }
                if(gameover == false){
                    gametime--;
                }
            }
            // 60 fps
            // OpenGL Draw commands
            player.set_position(player_x, player_y);
            head1.set_position(player_x, player_y);
            head2.set_position(imposter.position.x, imposter.position.y);
            if(player.position.x == imposter.position.x && player.position.y == imposter.position.y && vaporise == 0){
                gameover = true;
                win = 0;
            }
            if(player.position.x == button.position.x && player.position.y == button.position.y){
                vaporise = 1;
                tasks = 1;
            }
            if(player.position.x == button2.position.x && player.position.y == button2.position.y && activated == false){
                activated = true;
                tasks = 2;
            }
            if(player.position.x == coin1.position.x && player.position.y == coin1.position.y && activated == true && coin1val == true){
                coin1val = false;
                health += 5;
            }
            if(player.position.x == coin2.position.x && player.position.y == coin2.position.y && activated == true && coin2val == true){
                coin2val = false;
                health += 5;
            }
            if(player.position.x == bomb1.position.x && player.position.y == bomb1.position.y && activated == true && bomb1val == true){
                bomb1val = false;
                health -= 5;
            }
            if(health == 0){
                gameover = true;
                win = 0;
            }
            if(vaporise == 1 && activated == true && player.position.x == 2.0 && player.position.y == -2.0){
                gameover = true;
                win = 1;
            }
            if(gametime == 0){
                gameover = true;
                win = 0;
            }


            draw();

            if(win == 1){
                gltInit();
                GLTtext *wtext = gltCreateText();
                gltSetText(wtext, "You won :)");
                gltBeginDraw();
                gltColor(1.0f, 1.0f, 1.0f, 1.0f);
                gltDrawText2D(wtext, 160.0f, 280.0f, 3.0f);
                gltEndDraw();
                gltDeleteText(wtext);
                gltTerminate();
            }
            if(win == 0){
                gltInit();
                GLTtext *ltext = gltCreateText();
                gltSetText(ltext, "You lost :(");
                gltBeginDraw();
                gltColor(1.0f, 1.0f, 1.0f, 1.0f);
                gltDrawText2D(ltext, 160.0f, 280.0f, 3.0f);
                gltEndDraw();
                gltDeleteText(ltext);
                gltTerminate();
            }

            gltInit();
            GLTtext *healthtext = gltCreateText();
            char healthtextstr[15];
            sprintf(healthtextstr, "Health: %d", health);
            gltSetText(healthtext, healthtextstr);
            gltBeginDraw();
            gltColor(1.0f, 1.0f, 1.0f, 1.0f);
            gltDrawText2D(healthtext, 55.0f, 80.0f, 2.0f);
            gltEndDraw();
            gltDeleteText(healthtext);
            gltTerminate();

            gltInit();
            GLTtext *timetext = gltCreateText();
            char timetextstr[15];
            sprintf(timetextstr, "Time: %d", gametime);
            gltSetText(timetext, timetextstr);
            gltBeginDraw();
            gltColor(1.0f, 1.0f, 1.0f, 1.0f);
            gltDrawText2D(timetext, 400.0f, 80.0f, 2.0f);
            gltEndDraw();
            gltDeleteText(timetext);
            gltTerminate();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            //tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
