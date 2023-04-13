#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "aircraft_game_object.h"

namespace game {

// Inherits from GameObject
class EnemyGameObject : public AircraftGameObject {
   public:
    EnemyGameObject(const glm::vec3 &position, GLuint texture, const glm::vec3 &orbitPoint, GameObject *target);

    void Update(double delta_time) override;

    inline glm::vec3& GetOrbit(void) { return orbitPoint_; }
    inline GameObject* GetTarget(void) { return target_; }

    inline void SetOrbit(const glm::vec3& orbit) { orbitPoint_= orbit; }
    inline void SetTarget(GameObject* target) { target_ = target; }

   private:
    void patroll(void);
    void move(void);

    int state;
    glm::vec3 orbitPoint_;
    GameObject *target_;
};

}  // namespace game

#endif  // ENEMY_GAME_OBJECT_H_