#include <cmath>
#include "collectible_game_object.h"
#include <iostream>

namespace game {

CollectibleGameObject::CollectibleGameObject(const glm::vec3 &position, GLuint texture, ObjTypes type)
	: GameObject(position, texture) {
        type_ = type;
        startTime = 0.0;
}

void CollectibleGameObject::Update(double delta_time) {
    startTime += delta_time;
    double mResult = fmod(startTime, 1.0);
    std::cout << mResult << "\n";
    if (mResult > 0.5){
        velocity_.y = 0.5;
    } else{
        velocity_.y = -0.5;
    }
    GameObject::Update(delta_time);
}

bool CollectibleGameObject::Collide(GameObject* other) {
    if (!other->isKill() && other->getCollisionType() == 1){
        if(glm::distance(position_, other->GetPosition()) < 0.5f){
            return true;
        }
    }
    return false;
}

} // namespace game