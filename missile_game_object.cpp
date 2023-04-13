#include "missile_game_object.h"

namespace game {

MissileGameObject::MissileGameObject(const glm::vec3 &position, GLuint texture, GameObject *dontKillMePlz, GameObject *target)
    : GameObject(position, texture) {
    target_ = target;
    firer = dontKillMePlz;
    collisionType_ = 3;
    current_t_ = 0.0;
	start_time = 0.0;
    last_t_ = 0.0;
}

bool MissileGameObject::Collide(GameObject *other){
    if (other != firer && other->getCollisionType() == 1){
        if(glm::distance(position_, other->GetPosition()) < 0.2f){
            deceased_ = true;
            // other->kill(true);
            other->setHealth(0.0f);
            return true;
        }
    }
    return false;
}

void MissileGameObject::Update(double delta_time) {

    rotation_ = atan2(target_->GetPosition().y - position_.y, target_->GetPosition().x - position_.x) - M_PI / 2;
    velocity_ = glm::vec3(-2.0 * glm::sin(rotation_), 2.0 * glm::cos(rotation_), 0);

    last_t_ = current_t_;
    current_t_ += delta_time;
	
	if(current_t_ - start_time > 8){
		deceased_ = true;
	}

    
    GameObject::Update(delta_time);
    // TODO: bullet should be deleted after some time
}

}  // namespace game