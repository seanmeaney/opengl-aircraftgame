#include "player_game_object.h"

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, GLuint texture, GLuint bladeTexture)
	: AircraftGameObject(position, texture) {
        blades = new GameObject(glm::vec3(0.0f,0.0f,-0.1f), bladeTexture);
        blades->setParent(this);
        subObjects.push_back(blades);
        health_ = PLAYER_STARTING_HEALTH;
    }

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {

    blades->SetAngle(blades->GetAngle() - (15.0f * delta_time));
	// Call the parent's update method to move the object in standard way, if desired
	AircraftGameObject::Update(delta_time);
}

} // namespace game
