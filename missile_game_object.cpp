#include "missile_game_object.h"

namespace game {

MissileGameObject::MissileGameObject(const glm::vec3 &position, GLuint texture, GameObject *dontKillMePlz, GameObject *target)
    : GameObject(position, texture) {
    target_ = target;
    firer = dontKillMePlz;
    collisionType_ = MISSILECOLLISION;
    current_t_ = 0.0;
	start_time = 0.0;
    last_t_ = 0.0;
}

bool MissileGameObject::Collide(GameObject *other){
    if (other != firer && other->getCollisionType() == STANDARDCOLLISION){
        if(glm::distance(position_, other->GetPosition()) < 0.2f){
            deceased_ = true;
            childParticles_->kill(true);
            other->setHealth(0.0f);
            return true;
        }
    }
    return false;
}

void MissileGameObject::setChildParticles(GameObject * c){
    childParticles_ = c;
}

void MissileGameObject::Update(double delta_time) {

    rotation_ = atan2(target_->GetPosition().y - position_.y, target_->GetPosition().x - position_.x) - M_PI / 2;
    velocity_ = glm::vec3(-MISSILE_SPEED * glm::sin(rotation_), MISSILE_SPEED * glm::cos(rotation_), 0);

    last_t_ = current_t_;
    current_t_ += delta_time;
	
	if(current_t_ - start_time > MISSILE_MAX_FLIGHT){
		deceased_ = true;
	}

    GameObject::Update(delta_time);
}

}  // namespace game