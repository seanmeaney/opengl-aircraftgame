#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class ParticleSystem : public GameObject {

        public:
            ParticleSystem(const glm::vec3 &position, GLuint texture, GameObject *parent);

            void Update(double delta_time) override;

            void Render(Shader &shader, double current_time);

            inline void setCycle(float c) { cycle_ = c;}

        private:
            GameObject *parent_;
            float cycle_;

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_
