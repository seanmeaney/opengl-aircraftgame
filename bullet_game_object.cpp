#include "bullet_game_object.h"

namespace game {

BulletGameObject::BulletGameObject(const glm::vec3 &position, GLuint texture, GameObject *dontKillMePlz)
    : GameObject(position, texture) {
    initial_pos_ = position;
    current_t_ = 0.0;
	start_time = 0.0;
    last_t_ = 0.0;
    collisionType_ = 2;
    firer = dontKillMePlz;
}

bool BulletGameObject::Collide(GameObject *other) {
    if (!deceased_ && !other->isKill() && other->getCollisionType() == 1 && other != firer) {
        // Define terms of quadratic equation for ray-circle intersection test
        // We assume that they ray direction is given by the velocity vector
        glm::vec3 pmc(initial_pos_ - other->GetPosition());
        float a = glm::dot(velocity_, velocity_);
        float b = glm::dot(2.0f * velocity_, pmc);
        // May need to fine-tune the default radius for best effects
        float r = 0.5;  // obj[i]->GetRadius();
        float c = glm::dot(pmc, pmc) - r * r;

        // Solve quadratic equation
        float det = b * b - 4.0f * a * c;

        // If we have potential intersections,
        // find value of ray parameters "t" where intersection happens
        float t1 = (-b + sqrt(det)) / (2 * a);
        float t2 = (-b - sqrt(det)) / (2 * a);

        if ((t1 > last_t_) && (t1 < current_t_) || (t2 > last_t_) && (t2 < current_t_)) {
            deceased_ = true;
            // other->kill(true);
			other->setHealth(other->getHealth() - 25.0f);
			return true;
        }
    }
	return false;
}

void BulletGameObject::Update(double delta_time) {
    // Call the parent's update method to move the object in standard way, if desired
    GameObject::Update(delta_time);

	// Update current and previous time
    last_t_ = current_t_;
    current_t_ += delta_time;
	
	if(current_t_ - start_time > 3){
		deceased_ = true;
	}

    

    // TODO: bullet should be deleted after some time
}

}  // namespace game