#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>
#include <random>

#include <path_config.h>

#include "shader.h"

#include "game.h"

namespace game {

const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
const char *window_title_g = "Aircraft Game";  
// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;

Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set window to not resizable
    // Required or else the calculation to get cursor pos to screenspace will be incorrect
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Initialize particle shader
    particle_shader_.Init((resources_directory_g+std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/particle_fragment_shader.glsl")).c_str());
    particle_shader_.CreateParticles();

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());
    sprite_shader_.CreateSprite();
    sprite_shader_.Enable();
    sprite_shader_.SetSpriteAttributes();

    // Initialize time
    current_time_ = 0.0;
}

Game::~Game()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Game::mouseCallback(GLFWwindow* window, double xpos, double ypos){
    CursorGameObject *cur = static_cast<CursorGameObject *>(glfwGetWindowUserPointer(window));
    cur->mouseCallback(xpos, ypos);
}


void Game::Setup(void)
{
    // Load textures
    SetAllTextures();

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    player_ = new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex_[TEX_CHOPPER], tex_[TEX_BLADE]);

    // Setup particle system
    ParticleSystem *particles = new ParticleSystem(glm::vec3(0.0f, 0.0f, -0.1f), tex_[TEX_ORB], player_);
    particles->setCollsionType(0);
    particles->SetScale(0.2);
    particles->setCycle(0.4);
    game_objects_.push_back(particles);
    game_objects_.push_back(player_);
    

    cursor_ = new CursorGameObject(glm::vec3(0.0f, 0.0f, -0.2f),tex_[TEX_CURSOR], tex_[TEX_CURSOR_LOCK], player_);
    cursor_->setShadow(false);

    // Set event callbacks
    // glfwSetFramebufferSizeCallback(window_, ResizeCallback);
    glfwSetWindowUserPointer(window_, cursor_);
    glfwSetCursorPosCallback(window_, mouseCallback);

    // Setup background
    background = new GameObject(glm::vec3(0.0f, 0.0f, 0.2f), tex_[TEX_DUNE]);
    background->setCollsionType(0);
    background->SetScale(100.0);
    background->setFill(false);
    background->setShadow(false);

    generateEnemies();
    generateCollectibles();
    superHackeyHudThing();
}

void Game::generateEnemies(void){
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> rndEnemyNum(GAME_MIN_NUM_ENEMIES, GAME_MAX_NUM_ENEMIES);
    std::uniform_int_distribution<int> rngPos(-GAME_ENEMY_SPAWN_RADIUS, GAME_ENEMY_SPAWN_RADIUS);
    float tempx;
    float tempy;
    for (int i = 0; i < rndEnemyNum(rng); i++)
    {
        tempx = (float) rngPos(rng);
        tempy = (float) rngPos(rng);

        game_objects_.push_back(new EnemyGameObject(glm::vec3(tempx, tempy, 0.0f), tex_[TEX_REDPLANE], glm::vec3(tempx + 4.0f, tempy - 4.0f, 0.0f), player_));
        tempx = (float) rngPos(rng);
        tempy = (float) rngPos(rng);
        game_objects_.push_back(new EnemyGameObject(glm::vec3(tempx, tempy, 0.0f), tex_[TEX_GREENPLANE], glm::vec3(tempx + 4.0f, tempy - 4.0f, 0.0f), player_));

        CollectibleGameObject *tempttt = new CollectibleGameObject(glm::vec3((float) rngPos(rng), (float) rngPos(rng), 0.0f),tex_[TEX_MISSILE], MissileType);
        tempttt->SetScale(0.4);
        collectibles_.push_back(tempttt);
    }
}

void Game::generateCollectibles(void){
    collectibles_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, -3.0f, 0.0f), tex_[TEX_ROCKET_BODY], RocketBodyType));
    collectibles_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, 3.0f, 0.0f), tex_[TEX_BOOSTER], RocketBoosterType));
    collectibles_.push_back(new CollectibleGameObject(glm::vec3(3.0f, 3.0f, 0.0f), tex_[TEX_BOOSTER], RocketBoosterType));
    collectibles_.push_back(new CollectibleGameObject(glm::vec3(3.0f, -3.0f, 0.0f), tex_[TEX_GAS], RocketFuel));
}

void Game::superHackeyHudThing(void){
    glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom)) *
                            glm::translate(glm::mat4(1.0f), -player_->GetPosition());

    float aspect_ratio = ((float) window_width_g)/((float) window_height_g);

    // Set view to zoom out, centered by default at 0,0
    glm::mat4 window_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    staticViewMatrix = window_scale * view_matrix;

    headsUD = new HUD(tex_[TEX_ROCKET_BODY], tex_[TEX_BOOSTER], tex_[TEX_GAS], tex_[TEX_MISSILE], tex_[TEX_HUD]);
}


void Game::MainLoop(void)
{

    // Loop while the user did not close the window
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Clear background
        glClearColor(viewport_background_color_g.r,
                     viewport_background_color_g.g,
                     viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view to zoom out, centered by default at 0,0

        glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom)) *
                                glm::translate(glm::mat4(1.0f), -player_->GetPosition());

        float aspect_ratio = ((float) window_width_g)/((float) window_height_g);

        // Set view to zoom out, centered by default at 0,0
        glm::mat4 window_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
        playerViewMatrix = window_scale * view_matrix;
        particle_shader_.SetUniformMat4("view_matrix", playerViewMatrix);
        
        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update (and render) the game
        Update(deltaTime);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::SetTexture(GLuint w, const char *fname, GLint clamp = GL_CLAMP_TO_EDGE) {
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char *image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::SetAllTextures(void)
{
    // Load all textures that we will need
    glGenTextures(NUM_TEXTURES, tex_);
    SetTexture(tex_[0], (resources_directory_g+std::string("/textures/chopper.png")).c_str());
    SetTexture(tex_[1], (resources_directory_g+std::string("/textures/plane_red.png")).c_str());
    SetTexture(tex_[2], (resources_directory_g+std::string("/textures/plane_green.png")).c_str());
    SetTexture(tex_[3], (resources_directory_g+std::string("/textures/dune.png")).c_str(), GL_MIRRORED_REPEAT);
    SetTexture(tex_[4], (resources_directory_g+std::string("/textures/orb.png")).c_str());
    SetTexture(tex_[5], (resources_directory_g+std::string("/textures/blade.png")).c_str());
    SetTexture(tex_[6], (resources_directory_g+std::string("/textures/bullet.png")).c_str());
    SetTexture(tex_[7], (resources_directory_g+std::string("/textures/aim.png")).c_str());
    SetTexture(tex_[8], (resources_directory_g+std::string("/textures/aim_lock.png")).c_str());
    SetTexture(tex_[9], (resources_directory_g+std::string("/textures/missile.png")).c_str());
    SetTexture(tex_[10], (resources_directory_g+std::string("/textures/rocket.png")).c_str());
    SetTexture(tex_[11], (resources_directory_g+std::string("/textures/gas.png")).c_str());
    SetTexture(tex_[12], (resources_directory_g+std::string("/textures/rocket_body.png")).c_str());
    SetTexture(tex_[13], (resources_directory_g+std::string("/textures/booster.png")).c_str());
    SetTexture(tex_[14], (resources_directory_g+std::string("/textures/hud.png")).c_str());
}

void Game::fireBullet(void){
    BulletGameObject* b = new BulletGameObject(player_->GetPosition(), tex_[TEX_BULLET], player_);
    b->SetVelocity(BULLET_SPEED * player_->GetBearing());
    b->SetScale(0.3);
    b->SetAngle(player_->GetAngle());
    game_objects_.push_back(b);
}

void Game::fireMissile(GameObject * target){
    MissileGameObject* m = new MissileGameObject(player_->GetPosition(),tex_[TEX_MISSILE],player_, target);
    m->SetScale(0.4);
    m->SetAngle(player_->GetAngle());

    ParticleSystem *particles = new ParticleSystem(glm::vec3(0.0f, -0.2f, 0.0f), tex_[TEX_ORB], m);
    particles->setCollsionType(0);
    particles->SetScale(0.2);
    particles->setCycle(1.2);
    game_objects_.push_back(particles);
    game_objects_.push_back(m);
    inv.numMissiles--;
}

void Game::Controls(void)
{

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player_->SetVelocity(player_->GetVelocity() + PLAYER_SPEED_MOVE * player_->GetBearing());
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player_->SetVelocity(player_->GetVelocity() - PLAYER_SPEED_MOVE * player_->GetBearing());
    }
    if (blastOffTime < 0 && glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player_->SetAngle(player_->GetAngle() - PLAYER_SPEED_ROTATE);
    }
    if (blastOffTime < 0 &&  glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player_->SetAngle(player_->GetAngle() + PLAYER_SPEED_ROTATE);
    }
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        if (player_->fire()){
            fireBullet();
        }
    }
    if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        GameObject *validTarget = cursor_->getTarget();
        if(validTarget && inv.numMissiles > 0 && player_->fire()){
            fireMissile(validTarget);
        }
    }

    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
}


void Game::Update(double delta_time)
{

    // Update time
    current_time_ += delta_time;

    // Handle user input
    Controls();

    // cursor_->Update(delta_time);

    background->Render(sprite_shader_, current_time_);

    // Update and render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        if(cursor_->Collide(current_game_object)){
            cursor_->setLock(current_game_object);
        }

        if (!current_game_object->isKill()){
            current_game_object->Update(delta_time);
            for (int j = i + 1; j < game_objects_.size(); j++) {
                current_game_object->Collide(game_objects_[j]);
            }
        } else if (current_game_object != player_) {
            game_objects_.erase(game_objects_.begin() + i);
            delete current_game_object;
            continue;
        } else {
            glfwSetWindowShouldClose(window_, true);
        }

        // Render all game objects
        ParticleSystem *p = dynamic_cast<ParticleSystem*>(current_game_object);
        if (p != nullptr){  //if game object is a particle system (as shown by cast)
            current_game_object->Render(particle_shader_, current_time_);
        } else {    //else render with normal shader
            sprite_shader_.SetUniformMat4("view_matrix", playerViewMatrix);
            current_game_object->Render(sprite_shader_, current_time_);
        }
    }

    for (int i = 0; i < collectibles_.size(); i++)
    {
        collectibles_[i]->Update(delta_time);
        collectibles_[i]->Render(sprite_shader_, current_time_);
        if(collectibles_[i]->Collide(player_)){
            CollectibleGameObject *tempCur = collectibles_[i];
            ObjTypes temp = tempCur->getType();
            if (temp == MissileType){
                inv.numMissiles++;
            } else if (temp == RocketBodyType){
                inv.rocketBody = true;
            } else if (temp == RocketBoosterType){
                inv.rocketBooster ++;
            } else if (temp == RocketFuel){
                inv.rocketFuel = true;
            }
            collectibles_.erase(collectibles_.begin() + i);
            delete tempCur;
        }
    }

    headsUD->update(inv);

    if(inv.rocketBody == true && inv.rocketBooster > 1 && inv.rocketFuel == true){
        blastOffTime = current_time_;
        player_ = new PlayerGameObject(player_->GetPosition(),tex_[TEX_ROCKET], tex_[TEX_ORB]);
        tempHackyEndGame_ = new ParticleSystem(glm::vec3(0.0f, -0.6f, 0.0f), tex_[TEX_ORB], player_);
        tempHackyEndGame_->setCollsionType(0);
        tempHackyEndGame_->SetScale(0.4);
        tempHackyEndGame_->setCycle(1.8);
    }
    if (blastOffTime > 0){
        glm::vec3 ouutatime = player_->GetPosition();
        ouutatime.y += 0.01f;
        player_->SetPosition(ouutatime);
        tempHackyEndGame_->Update(current_time_);
        tempHackyEndGame_->Render(particle_shader_, current_time_);
        player_->Render(sprite_shader_, current_time_);
        if (current_time_ - blastOffTime > GAME_ENDGAME_TIME){
            glfwSetWindowShouldClose(window_, true);
        }
    }


    cursor_->Render(sprite_shader_, current_time_);

    //hacky hud uses a seperate viewmatrix that is locked to the screen
    //must swap to the static viewmatrix before rendering
    sprite_shader_.SetUniformMat4("view_matrix", staticViewMatrix);
    headsUD->render(sprite_shader_, current_time_);
 
}
       
} // namespace game
