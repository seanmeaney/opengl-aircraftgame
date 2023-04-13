#include "collectible_game_object.h"

namespace game {

CollectibleGameObject::CollectibleGameObject(const glm::vec3 &position, GLuint texture, ObjTypes type)
	: GameObject(position, texture) {
        type_ = type;
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