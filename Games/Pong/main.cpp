#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <cmath>

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, player1Matrix, player2Matrix, ballMatrix, projectionMatrix;

//player 1
// Start at -4.5, 0, 0
glm::vec3 player1_position = glm::vec3(-4.5f, 0, 0);
// Dont go anywhere (yet).
glm::vec3 player1_movement = glm::vec3(0, 0, 0);

//player 2
// Start at 4.5, 0, 0
glm::vec3 player2_position = glm::vec3(4.5f, 0, 0);
// Dont go anywhere (yet).
glm::vec3 player2_movement = glm::vec3(0, 0, 0);

//ball
//Start at 0, 0, 0
glm::vec3 ball_position = glm::vec3(0, 0, 0);
// goin places.
glm::vec3 ball_movement = glm::vec3(1.5f, 2.0f, 0);

float player_start_speed = 4.0f;
float player_speed = 0;
float paddle_height = 2.0f;
float paddle_width = 1.0f;

float ball_start_speed = 1.5f;
float ball_speed = 0;
float ball_height = 1.0f;
float ball_width = 1.0f;

float right = 5.0f;
float left = -5.0f;
float top = 3.75f;
float bottom = -3.75f;

Mix_Music *music;
Mix_Chunk* bounce;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Pong - Press Space to Start or Pause Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.10f);

    bounce = Mix_LoadWAV("bounce.wav");


    viewMatrix = glm::mat4(1.0f);
    player1Matrix = glm::mat4(1.0f);
    player2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    /*program.SetColor(1.0f, 0.f, 0.0f, 1.0f);*/

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

//return the boundary values of object
float topOf(float y_position, float height) {
    return (y_position + height / 2);
}

float bottomOf(float y_position, float height) {
    return (y_position - height / 2);
}

float rightOf(float x_position, float width) {
    return (x_position + width / 2);
}

float leftOf(float x_position, float width) {
    return (x_position - width / 2);
}

//check if object is in bounds
bool isInBoundsTop(float y_position, float height) {
    if (topOf(y_position, height) > top) {
        return false;
    }
    return true;
}

bool isInBoundsBottom(float y_position, float height) {
    if (bottomOf(y_position, height) < bottom) {
        return false;
    }
    return true;
}

bool isInBoundsRight(float x_position, float width) {
    if (rightOf(x_position, width) > right) {
        return false;
    }
    return true;
}

bool isInBoundsLeft(float x_position, float width) {
    if (leftOf(x_position, width) < left) {
        return false;
    }
    return true;
}

void ProcessInput() {

    player1_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                // Start and pause game when space is pressed
                if (ball_speed == ball_start_speed) {
                    ball_speed = 0;
                    player_speed = 0;
                }
                else {
                    ball_speed = ball_start_speed;
                    player_speed = player_start_speed;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    //player 1 movement
    /*if (keys[SDL_SCANCODE_A]) {
        player1_movement.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_D]) {
        player1_movement.x = 1.0f;
    }*/

    if (keys[SDL_SCANCODE_W] && isInBoundsTop(player1_position.y, paddle_height)) {
        player1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S] && isInBoundsBottom(player1_position.y, paddle_height)) {
        player1_movement.y = -1.0f;
    }

    if (glm::length(player1_movement) > 1.0f) {
        player1_movement = glm::normalize(player1_movement);
    }

    //player 2 movement
    /*if (keys[SDL_SCANCODE_LEFT]) {
        player2_movement.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        player2_movement.x = 1.0f;
    }*/

    if (keys[SDL_SCANCODE_UP] && isInBoundsTop(player2_position.y, paddle_height)) {
        player2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN] && isInBoundsBottom(player2_position.y, paddle_height)) {
        player2_movement.y = -1.0f;
    }

    if (glm::length(player2_movement) > 1.0f) {
        player2_movement = glm::normalize(player2_movement);
    }

}

bool shouldBounceOffTop(float player_x, float player_y) {
    if ((bottomOf(ball_position.y, ball_height) <= topOf(player_y, paddle_height)) &&
        (topOf(ball_position.y, ball_height) > topOf(player_y, paddle_height))&&
        (rightOf(ball_position.x, ball_width) > leftOf(player_x, paddle_width)) &&
        (leftOf(ball_position.x, ball_width) < rightOf(player_x, paddle_width))) {
        return true;
    }
    return false;
}

bool shouldBounceOffBottom(float player_x, float player_y) {
    if ((topOf(ball_position.y, ball_height) >= bottomOf(player_y, paddle_height)) &&
        (bottomOf(ball_position.y, ball_height) < bottomOf(player_y, paddle_height)) &&
        (rightOf(ball_position.x, ball_width) > leftOf(player_x, paddle_width)) &&
        (leftOf(ball_position.x, ball_width) < rightOf(player_x, paddle_width))) {
        return true;
    }
    return false;
}

//bounce off of right side of player 1(left paddle)
bool shouldBounceOffRight(float player_x, float player_y) {
    if ((leftOf(ball_position.x, ball_width) <= rightOf(player_x, paddle_width) + 0.05f) &&
        (rightOf(ball_position.x, ball_width) > rightOf(player_x, paddle_width)) &&
        (topOf(ball_position.y, ball_height) > bottomOf(player_y, paddle_height)) &&
        (bottomOf(ball_position.y, ball_height) < topOf(player_y, paddle_height))) {
        return true;
    }
    return false;
}

//bounce off of left side of player 2(right paddle)
bool shouldBounceOffLeft(float player_x, float player_y) {
    if ((rightOf(ball_position.x, ball_width) >= leftOf(player_x, paddle_width) - 0.05f) &&
        (leftOf(ball_position.x, ball_width) < leftOf(player_x, paddle_width)) &&
        (topOf(ball_position.y, ball_height) > bottomOf(player_y, paddle_height)) &&
        (bottomOf(ball_position.y, ball_height) < topOf(player_y, paddle_height))) {
        return true;
    }
    return false;
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    //player 1 movement
    // Add (direction * units per second * elapsed time)
    player1_position += player1_movement * player_speed * deltaTime;
    player1Matrix = glm::mat4(1.0f);
    player1Matrix = glm::translate(player1Matrix, player1_position);

    //player 2 movement
    // Add (direction * units per second * elapsed time)
    player2_position += player2_movement * player_speed * deltaTime;
    player2Matrix = glm::mat4(1.0f);
    player2Matrix = glm::translate(player2Matrix, player2_position);

    //ball movement
    ball_position += ball_movement * ball_speed * deltaTime;
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ball_position);

    //ball bounce
    //bounce off top and bottom of screen
    if (!isInBoundsTop(ball_position.y, ball_height)) {
        ball_movement.y = -(abs(ball_movement.y));
    }
    else if (!isInBoundsBottom(ball_position.y, ball_height)) {
        ball_movement.y = abs(ball_movement.y);
    }

    //bounce off paddles
    //bounce off top of paddles
    if (shouldBounceOffTop(player1_position.x, player1_position.y) ||
        shouldBounceOffTop(player2_position.x, player2_position.y)) {
        ball_movement.y = abs(ball_movement.y);
    }
    //bounce off bottom of paddles
    else if (shouldBounceOffBottom(player1_position.x, player1_position.y) ||
        shouldBounceOffBottom(player2_position.x, player2_position.y)) {
        ball_movement.y = -(abs(ball_movement.y));
    }
    //bounce off side of left side of right paddle
    else if (shouldBounceOffLeft(player2_position.x, player2_position.y)) {
        ball_movement.x = -(abs(ball_movement.x));
    }
    //bounce off side of right side of left paddle
    else if (shouldBounceOffRight(player1_position.x, player1_position.y)) {
        ball_movement.x = abs(ball_movement.x);
    }

    //play sound
    if (!isInBoundsTop(ball_position.y, ball_height) ||
        !isInBoundsBottom(ball_position.y, ball_height) ||
        shouldBounceOffTop(player1_position.x, player1_position.y) ||
        shouldBounceOffTop(player2_position.x, player2_position.y) ||
        shouldBounceOffBottom(player1_position.x, player1_position.y) ||
        shouldBounceOffBottom(player2_position.x, player2_position.y) ||
        shouldBounceOffLeft(player2_position.x, player2_position.y) ||
        shouldBounceOffRight(player1_position.x, player1_position.y)) {
        Mix_PlayChannel(-1, bounce, 0);
    }
    

    //end game if ball reaches left or right wall
    if ((rightOf(ball_position.x, ball_width) >= right) || (leftOf(ball_position.x, ball_width) <= left)) {
        ball_speed = 0;
        ball_start_speed = 0;
        player_speed = 0;
        player_start_speed = 0;
    }
}

void DrawPlayer1() {
    program.SetModelMatrix(player1Matrix);
    //glBindTexture(GL_TEXTURE_2D, robotTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawPlayer2() {
    program.SetModelMatrix(player2Matrix);
    //glBindTexture(GL_TEXTURE_2D, profTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawBall() {
    program.SetModelMatrix(ballMatrix);
    //glBindTexture(GL_TEXTURE_2D, profTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetModelMatrix(player1Matrix);

    //float vertices[] = { 0.5f, -1.0f, 0.5f, 1.0f, -0.5f, 1.0f, -0.5f, 1.0f, -0.5f, -1.0f, 0.5f, -1.0f }; //height 2, width 1
    float paddleVertices[] = { (paddle_width/2), -(paddle_height/2), (paddle_width/2), (paddle_height / 2), -(paddle_width / 2), (paddle_height / 2), 
        -(paddle_width / 2), (paddle_height / 2), -(paddle_width / 2), -(paddle_height / 2), (paddle_width / 2), -(paddle_height / 2) }; //height 2, width 1

    float ballVertices[] = { (ball_width / 2), -(ball_height / 2), (ball_width / 2), (ball_height / 2), -(ball_width / 2), (ball_height / 2),
        -(ball_width / 2), (ball_height / 2), -(ball_width / 2), -(ball_height / 2), (ball_width / 2), -(ball_height / 2) }; //height 1, width 1

    //draw players
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddleVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    program.SetColor(1.0f, 1.f, 0.0f, 1.0f);
    DrawPlayer1();
    DrawPlayer2();

    //draw ball
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);
    program.SetColor(1.0f, 0.f, 0.0f, 1.0f);
    DrawBall();

    glDisableVertexAttribArray(program.positionAttribute);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}
