#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "aircraft_game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public AircraftGameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, GLuint texture, GLuint bladeTexture);

            // Update function for moving the player object around
            void Update(double delta_time) override;

        private:
            GameObject *blades;
    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
