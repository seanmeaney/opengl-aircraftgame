#ifndef MISSILE_GAME_OBJECT_H
#define MISSILE_GAME_OBJECT_H

#include "game_object.h"

namespace game {

    // Class that abstracts a bullet fired from a game object
    // Inherits from GameObject
    class MissileGameObject : public GameObject {

        public:
            MissileGameObject(const glm::vec3 &position, GLuint texture, GameObject *dontKillMePlz, GameObject *target);


            void setChildParticles(GameObject *);
            // Update function for moving the player object around
            void Update(double delta_time) override;

            bool Collide(GameObject *other) override;

            // int Collision(const std::vector<GameObject*> &obj);

        private:

            GameObject *firer;
            GameObject *target_;
            GameObject *childParticles_;

            float current_t_;

            // Time of the previous frame
            float last_t_;

            float start_time;

    }; // class Missile

} // namespace game

#endif // BULLET_H_
