#ifndef COLLECTIBLE_GAME_OBJECT_H
#define COLLECTIBLE_GAME_OBJECT_H

#include "game_object.h"
#include "defs.h"

namespace game {

    

    // Class that abstracts a bullet fired from a game object
    // Inherits from GameObject
    class CollectibleGameObject : public GameObject {

        public:
            CollectibleGameObject(const glm::vec3 &position, GLuint texture, ObjTypes type);

            // Update function for moving the player object around
            bool Collide(GameObject *other) override;

            inline ObjTypes getType(void) {return type_;}

        private:

            ObjTypes type_;



    }; // class CollectibleGameObject

} // namespace game

#endif