#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.h"
#include "player_game_object.h"
#include "enemy_game_object.h"
#include "particle_system.h"
#include "bullet_game_object.h"
#include "cursor_game_object.h"
#include "missile_game_object.h"
#include "collectible_game_object.h"
#include "HUD.h"
#include "game_object.h"
#include "defs.h"

namespace game {


    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game (scene, game objects, etc.)
            void Setup(void);

            // Run the game (keep the game active)
            void MainLoop(void); 

            void fireBullet(void);
            void fireMissile(GameObject *);
            void generateEnemies(void);
            void generateCollectibles(void);

            void superHackeyHudThing(void);

        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Shader for rendering sprites in the scene
            Shader sprite_shader_;

            // Shader for rendering particles
            Shader particle_shader_;

            // References to textures
#define NUM_TEXTURES 15
            GLuint tex_[NUM_TEXTURES];

            // List of game objects
            std::vector<GameObject*> game_objects_;
            std::vector<CollectibleGameObject*> collectibles_;
            Inventory inv;


            PlayerGameObject* player_;
            ParticleSystem * tempHackyEndGame_;
            GameObject *background;
            CursorGameObject* cursor_;
            HUD *headsUD;
            

            glm::mat4 playerViewMatrix; //moving view matrix with player in center
            glm::mat4 staticViewMatrix; //for hud and other game elements that remain in a single position on screen
            double blastOffTime = -1.0;

            // Keep track of time
            double current_time_;

            // Callback for when the window is resized
            // static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname, GLint clamp);

            // Load all textures
            void SetAllTextures();

            // Handle user input
            void Controls(void);

            // Update the game based on user input and simulation
            void Update(double delta_time);

    }; // class Game

} // namespace game

#endif // GAME_H_
