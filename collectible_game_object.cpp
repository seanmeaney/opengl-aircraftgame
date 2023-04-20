#include "collectible_game_object.h"

namespace game {

CollectibleGameObject::CollectibleGameObject(const glm::vec3 &position, GLuint texture, ObjTypes type)
	: GameObject(position, texture) {
        type_ = type;
        startPosition = position;
}

void CollectibleGameObject::Update(double delta_time) {
    if (position_.y - startPosition.y > 0.1){
        velocity_.y -= 0.03;
    } else if (position_.y - startPosition.y < 0.05) {
        velocity_.y += 0.03;
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