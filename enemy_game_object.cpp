#include <cmath>
#include <iostream>
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
    SetAngle(atan2(position_.y - orbitPoint_.y, position_.x - orbitPoint_.x));
    velocity_ = glm::vec3(-ENEMY_SPEED_MOVE * glm::sin(rotation_), ENEMY_SPEED_MOVE * glm::cos(rotation_), 0);
}

void EnemyGameObject::move(void) {
    float angleToTarget = atan2(target_->GetPosition().y - position_.y, target_->GetPosition().x - position_.x) - M_PI / 2;
    angleToTarget = fmod(angleToTarget, 2 * M_PI);
    if (angleToTarget < 0.0){
        angleToTarget += 2 * M_PI;
    }
    float allowedAngleDelta = fmod((rotation_ - angleToTarget), ENEMY_SPEED_ROTATE);
    SetAngle(rotation_ - allowedAngleDelta);
    velocity_ = glm::vec3(-ENEMY_SPEED_MOVE * glm::sin(rotation_), ENEMY_SPEED_MOVE * glm::cos(rotation_), 0);
}

bool EnemyGameObject::shoot(void){
    float angleToTarget = atan2(target_->GetPosition().y - position_.y, target_->GetPosition().x - position_.x) - M_PI / 2;
    if (glm::abs((angleToTarget - rotation_) < ENEMY_SHOOT_MAX_ANGLE) && glm::distance (position_, target_->GetPosition()) < ENEMY_SHOOT_DISTANCE){
        return fire();
    }
    return false;
}

// Update function for moving the enemy object around
void EnemyGameObject::Update(double delta_time) {
    if (!deceased_) {
        if (glm::distance(position_, target_->GetPosition()) > ENEMY_TARGET_DISTANCE) {
            patroll();
        } else {
            move();
        }

        // Call the parent's update method to move the object in standard way, if desired
        AircraftGameObject::Update(delta_time);
    }
}

}  // namespace game
