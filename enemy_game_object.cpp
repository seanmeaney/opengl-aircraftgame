#include "defs.h"
#include "enemy_game_object.h"

namespace game {

/*
                EnemyGameObject inherits from GameObject
        */

EnemyGameObject::EnemyGameObject(const glm::vec3 &position, GLuint texture, const glm::vec3 &orbitPoint, GameObject *target)
    : AircraftGameObject(position, texture) {
    state = 0;
    orbitPoint_ = orbitPoint;
    target_ = target;
}

void EnemyGameObject::patroll(void) {
    rotation_ = atan2(position_.y - orbitPoint_.y, position_.x - orbitPoint_.x);
    velocity_ = glm::vec3(-ENEMY_SPEED_MOVE * glm::sin(rotation_), ENEMY_SPEED_MOVE * glm::cos(rotation_), 0);
}

void EnemyGameObject::move(void) {
    rotation_ = atan2(target_->GetPosition().y - position_.y, target_->GetPosition().x - position_.x) - M_PI / 2;
    velocity_ = glm::vec3(-ENEMY_SPEED_MOVE * glm::sin(rotation_), ENEMY_SPEED_MOVE * glm::cos(rotation_), 0);
}

// Update function for moving the enemy object around
void EnemyGameObject::Update(double delta_time) {
    if (!deceased_) {
        if (glm::distance(position_, target_->GetPosition()) > ENEMY_TARGET_DISTANCE) {
            state = 0;
        } else {
            state = 1;
        }

        if (state == 0) {
            patroll();
        } else {
            move();
        }

        // Call the parent's update method to move the object in standard way, if desired
        GameObject::Update(delta_time);
    }
}

}  // namespace game
