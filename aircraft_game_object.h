#ifndef AIRCRAFT_GAME_OBJECT_H_
#define AIRCRAFT_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class AircraftGameObject : public GameObject {

        public:
            AircraftGameObject(const glm::vec3 &position, GLuint texture);

            // Update function for moving the Aircraft object around
            void Update(double delta_time) override;

            bool fire(void);

        private:
            double fireCoolDown;

    }; // class AircraftGameObject

} // namespace game

#endif // Aircraft_GAME_OBJECT_H_
