#include "aircraft_game_object.h"

namespace game
{

    AircraftGameObject::AircraftGameObject(const glm::vec3 &position, GLuint texture)
        : GameObject(position, texture)
    {
        fireCoolDown = 0.0f;
    }

    bool AircraftGameObject::fire(void){
        if (fireCoolDown <= 0.0f){
            fireCoolDown = BULLET_COOLDOWN;
            return true;
        }
        else{
            return false;
        }
    }

    void AircraftGameObject::Update(double delta_time){
        if (glm::length(velocity_) > AIRCRAFT_VELOCITY_CAP) {
            velocity_ = glm::normalize(velocity_) * AIRCRAFT_VELOCITY_CAP;
        }

        if(fireCoolDown > 0.0f){
            fireCoolDown -= delta_time;
        }

        // Call the parent's update method to move the object in standard way, if desired
        GameObject::Update(delta_time);
    }

} // namespace game
