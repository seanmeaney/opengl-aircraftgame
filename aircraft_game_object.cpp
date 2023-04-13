#include "aircraft_game_object.h"

namespace game
{

    /*
        AircraftGameObject inherits from GameObject
        It overrides GameObject's update method, so that you can check for input to change the velocity of the Aircraft
    */

    AircraftGameObject::AircraftGameObject(const glm::vec3 &position, GLuint texture)
        : GameObject(position, texture)
    {
        fireCoolDown = 0.0f;
    }

    bool AircraftGameObject::fire(void)
    {
        if (fireCoolDown <= 0.0f)
        {
            fireCoolDown = 0.4f;
            return true;
        }
        else
        {
            return false;
        }
    }

    // Update function for moving the Aircraft object around
    void AircraftGameObject::Update(double delta_time)
    {
        if (glm::length(velocity_) > 1.8f) {
        velocity_ = glm::normalize(velocity_) * 1.8f;
        }
        // yes this does nothing, allows change in direction
        else if (glm::length(velocity_) < 0.0f) {
            velocity_ = glm::normalize(velocity_) * 0.0f;
        }

        if(fireCoolDown > 0.0f){
            fireCoolDown -= delta_time;
        }

        // Call the parent's update method to move the object in standard way, if desired
        GameObject::Update(delta_time);
    }

} // namespace game
