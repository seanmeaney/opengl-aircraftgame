#ifndef BULLET_GAME_OBJECT_H
#define BULLET_GAME_OBJECT_H

#include "game_object.h"

namespace game {

    // Class that abstracts a bullet fired from a game object
    // Inherits from GameObject
    class BulletGameObject : public GameObject {

        public:
            BulletGameObject(const glm::vec3 &position, GLuint texture, GameObject *dontKillMePlz);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            bool Collide(GameObject *other) override;

            // int Collision(const std::vector<GameObject*> &obj);

        private:
            // Initial position of the ray
            // This is different from the current position stored in position_
            glm::vec3 initial_pos_;

            GameObject *firer;

            // Time of current frame since the creation of the bullet
            float current_t_;

            // Time of the previous frame
            float last_t_;

            float start_time;

    }; // class Bullet

} // namespace game

#endif // BULLET_H_
