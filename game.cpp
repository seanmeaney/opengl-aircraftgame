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

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Final Project";
const unsigned int window_width_g = 800*2;
const unsigned int window_height_g =  600*2;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);

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

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

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


void Game::Setup(void)
{

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(50, 75);
    std::uniform_int_distribution<int> un2(-40, 40);

    // Load textures
    SetAllTextures();

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    player_ = new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex_[0], tex_[5]);

    // Setup particle system
    ParticleSystem *particles = new ParticleSystem(glm::vec3(0.0f, 0.0f, -0.1f), tex_[4], player_);
    particles->setCollsionType(0);
    particles->SetScale(0.2);
    particles->setCycle(0.4);
    game_objects_.push_back(particles);
    game_objects_.push_back(player_);
    

    cursor_ = new CursorGameObject(glm::vec3(0.0f, 0.0f, -0.2f),tex_[7], tex_[8], window_, player_);
    // game_objects_.push_back(cursor_);

    // Setup background
    background = new GameObject(glm::vec3(0.0f, 0.0f, 0.1f), tex_[3]);
    background->setCollsionType(0);
    background->SetScale(100.0);
    background->setFill(false);


    float tempx;
    float tempy;
    for (int i = 0; i < uni(rng); i++)
    {
        tempx = (float) un2(rng);
        tempy = (float) un2(rng);

        game_objects_.push_back(new EnemyGameObject(glm::vec3(tempx, tempy, 0.0f), tex_[1], glm::vec3(tempx + 4.0f, tempy - 4.0f, 0.0f), player_));
        tempx = (float) un2(rng);
        tempy = (float) un2(rng);
        game_objects_.push_back(new EnemyGameObject(glm::vec3(tempx, tempy, 0.0f), tex_[2], glm::vec3(tempx + 4.0f, tempy - 4.0f, 0.0f), player_));

        CollectibleGameObject *tempttt = new CollectibleGameObject(glm::vec3((float) un2(rng), (float) un2(rng), 0.0f),tex_[9], MissileType);
        tempttt->SetScale(0.4);
        collectibles_.push_back(tempttt);
    }
    
    collectibles_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, -3.0f, 0.0f), tex_[12], RocketBodyType));
    collectibles_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, 3.0f, 0.0f), tex_[13], RocketBoosterType));
    collectibles_.push_back(new CollectibleGameObject(glm::vec3(3.0f, 3.0f, 0.0f), tex_[13], RocketBoosterType));
    collectibles_.push_back(new CollectibleGameObject(glm::vec3(3.0f, -3.0f, 0.0f), tex_[11], RocketFuel));

    headsUD = new HUD(tex_[12], tex_[13], tex_[11], tex_[9], tex_[14]);
}


void Game::superHackeyHudThing(void){
    float cameraZoom = 0.25f;
    glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) *
                            glm::translate(glm::mat4(1.0f), -player_->GetPosition());

    float aspect_ratio = ((float) window_width_g)/((float) window_height_g);

    // Set view to zoom out, centered by default at 0,0
    glm::mat4 window_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    temp2 = window_scale * view_matrix;
}


void Game::MainLoop(void)
{

    superHackeyHudThing();

    // Loop while the user did not close the window
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Clear background
        glClearColor(viewport_background_color_g.r,
                     viewport_background_color_g.g,
                     viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view to zoom out, centered by default at 0,0

        float cameraZoom = 0.25f;
        glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) *
                                glm::translate(glm::mat4(1.0f), -player_->GetPosition());

        float aspect_ratio = ((float) window_width_g)/((float) window_height_g);

        // Set view to zoom out, centered by default at 0,0
        glm::mat4 window_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
        temp1 = window_scale * view_matrix;
        // glm::mat4 temp2 = window_scale * view_matrix;
        // sprite_shader_.SetUniformMat4("view_matrix", temp1);
        particle_shader_.SetUniformMat4("view_matrix", temp1);
        
        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the game
        Update(deltaTime);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
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
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}


void Game::Controls(void)
{

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player_->SetVelocity(player_->GetVelocity() + 0.01f * player_->GetBearing());
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player_->SetVelocity(player_->GetVelocity() - 0.01f * player_->GetBearing());
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player_->SetAngle(player_->GetAngle() - 0.02);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player_->SetAngle(player_->GetAngle() + 0.02);
    }
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        if (player_->fire()){
            BulletGameObject* b = new BulletGameObject(player_->GetPosition(), tex_[6], player_);
            b->SetVelocity(5.0f * player_->GetBearing());
            b->SetScale(0.3);
            b->SetAngle(player_->GetAngle());
            game_objects_.push_back(b);
        }
    }
    if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        GameObject *validTarget = cursor_->getTarget();
        if(validTarget && inv.numMissiles > 0 && player_->fire()){
            MissileGameObject* m = new MissileGameObject(player_->GetPosition(),tex_[9],player_,validTarget);
            m->SetScale(0.4);
            m->SetAngle(player_->GetAngle());

            ParticleSystem *particles = new ParticleSystem(glm::vec3(0.0f, -0.2f, 0.0f), tex_[4], m);
            particles->setCollsionType(0);
            particles->SetScale(0.2);
            particles->setCycle(1.2);
            game_objects_.push_back(particles);
            game_objects_.push_back(m);
            inv.numMissiles--;
            
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

    bool cursorLock = false;
    cursor_->Update(delta_time);

    background->Render(sprite_shader_, current_time_);

    // Update and render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        if(cursor_->Collide(current_game_object)){
            cursorLock = true;
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

        // Render game object
        ParticleSystem *p = dynamic_cast<ParticleSystem*>(current_game_object);
        if (p != nullptr){
            current_game_object->Render(particle_shader_, current_time_);
        } else {
            sprite_shader_.SetUniformMat4("view_matrix", temp1);
            current_game_object->Render(sprite_shader_, current_time_);
        }
    }
    if(!cursorLock) {
        cursor_->setLock(nullptr);
    }

    for (int i = 0; i < collectibles_.size(); i++)
    {
        collectibles_[i]->Render(sprite_shader_, current_time_);
        if(collectibles_[i]->Collide(player_)){
            CollectibleGameObject *tempCur = collectibles_[i];
            ObjTypes temp = tempCur->getType();
            if (temp == MissileType){ // missile
                inv.numMissiles++;
            } else if (temp == RocketBodyType){ //rocketbody
                inv.rocketBody = true;
            } else if (temp == RocketBoosterType){ //rocketbooster
                inv.rocketBooster ++;
            } else if (temp == RocketFuel){ //rocketfuel
                inv.rocketFuel = true;
            }
            // inv.numMissiles++;
            collectibles_.erase(collectibles_.begin() + i);
            delete tempCur;
        }
    }
    
    headsUD->showMissile(inv.numMissiles > 0);
    headsUD->showBody(inv.rocketBody == true);
    headsUD->showB1(inv.rocketBooster > 0);
    headsUD->showb2(inv.rocketBooster > 1);
    headsUD->showFuel(inv.rocketFuel == true);

    if(inv.rocketBody == true && inv.rocketBooster > 1 && inv.rocketFuel == true){
        blastOff();
    }


    cursor_->Render(sprite_shader_, current_time_);

    //uhhhh
    sprite_shader_.SetUniformMat4("view_matrix", temp2);
    headsUD->render(sprite_shader_, current_time_);


    
    
}


void Game::blastOff(){

    GameObject *blast = new GameObject(player_->GetPosition(),tex_[10]);
    ParticleSystem *pow = new ParticleSystem(glm::vec3(0.0f, -0.2f, 0.0f), tex_[4], blast);
    pow->SetScale(0.4);
    pow->setCycle(1.8);


    double lastTime = glfwGetTime();
    for (int i = 0; i < 400; i++)
    {
        glClearColor(viewport_background_color_g.r,
                     viewport_background_color_g.g,
                     viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view to zoom out, centered by default at 0,0

        float cameraZoom = 0.25f;
        glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) *
                                glm::translate(glm::mat4(1.0f), -player_->GetPosition());

        float aspect_ratio = ((float) window_width_g)/((float) window_height_g);

        // Set view to zoom out, centered by default at 0,0
        glm::mat4 window_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
        temp1 = window_scale * view_matrix;
        // glm::mat4 temp2 = window_scale * view_matrix;
        // sprite_shader_.SetUniformMat4("view_matrix", temp1);
        
        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;


        glm::vec3 ouutatime = blast->GetPosition();
        ouutatime.y += 0.01f;
        blast->SetPosition(ouutatime);

        background->Render(sprite_shader_, current_time_);

        particle_shader_.SetUniformMat4("view_matrix", temp1);
        pow->Update(deltaTime);
        pow->Render(particle_shader_, current_time_);

        sprite_shader_.SetUniformMat4("view_matrix", temp1);
        blast->Render(sprite_shader_, current_time_);

        
        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        glfwPollEvents();
    }
    


    glfwSetWindowShouldClose(window_, true);
}
       
} // namespace game
