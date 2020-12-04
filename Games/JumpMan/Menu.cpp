#include "Menu.h"

#define Menu_ENEMY_COUNT 1

#define Menu_WIDTH 14
#define Menu_HEIGHT 8
unsigned int Menu_data[] =
{
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Menu::Initialize() {
    //setLives()
    state.sceneNum = 0;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.10f);

    jump = Mix_LoadWAV("bounce.wav");
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(Menu_WIDTH, Menu_HEIGHT, Menu_data, mapTextureID, 1.0f, 4, 1);

    fontTextureID = Util::LoadTexture("font1.png");

    // Initialize game objects
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.player->jumpPower = 5.0f;

    state.enemies = new Entity[Menu_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(1, 0, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;

    stopMotion();
}

void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, Menu_ENEMY_COUNT, state.map);

    for (int i = 0; i < Menu_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, Menu_ENEMY_COUNT, state.map);
    }

    //triggers moving to next scene
    if (state.player->position.x >= 12) {
        state.nextScene = 1;
    }
}

void Menu::playJumpSound() {
    Mix_PlayChannel(-1, jump, 0);
}

void Menu::Render(ShaderProgram* program) {
    state.map->Render(program);
    for (int i = 0; i < Menu_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    Util::DrawText(program, fontTextureID, "Jump Man", 0.75f, -0.25f, glm::vec3(3.5f, -2.25f, 0));
    Util::DrawText(program, fontTextureID, "Press 'Enter' to start", 0.5f, -0.25f, glm::vec3(2.6f, -3.0f, 0));
    Util::DrawText(program, fontTextureID, "Lives: " + std::to_string(state.lives), 0.5f, -0.25f, glm::vec3(state.player->position.x + 3.0f, -0.5f, 0));

    state.player->Render(program);
}

int Menu::setLives(int num) {
    state.lives = num;
    return state.lives;
}

int Menu::getLives() {
    return state.lives;
}

int Menu::loseLife() {
    state.lives--;
    return state.lives;
    //if (lives == 0) gameOver();
}

void Menu::stopMotion() {
    state.player->speed = 0;
    state.player->jumpPower = 0;

    for (int i = 0; i < Menu_ENEMY_COUNT; i++) {
        state.enemies->speed = 0;
    }
}