#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <vector>
#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of objects in the game world
        The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            GameObject(const glm::vec3 &position, GLuint texture);

            // Update the GameObject's state. Can be overriden for children
            virtual void Update(double delta_time);

            // Renders the GameObject using a shader
            virtual void Render(Shader &shader, double current_time);

            virtual bool Collide(GameObject *other);

            // Getters
            inline glm::vec3& GetPosition(void) { return position_; }
            inline float GetScale(void) { return scale_; }
            inline float GetAngle(void) { return rotation_; }
            inline glm::vec3& GetVelocity(void) { return velocity_; }
            glm::vec3 GetBearing(void);
            glm::vec3 GetRight(void);
            inline int getCollisionType(void) {return collisionType_;}
            inline bool isKill(void) { return deceased_; }
            inline float getHealth(void) { return health_;}


            inline void kill(bool k) { deceased_ = k;}
            inline void setParent(GameObject *p) {parent = p;}
            inline void setHealth (float h) { health_ = h;}

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(float scale) { scale_ = scale; }
            inline void setFill(bool f) {fill = f;}
            void SetAngle(float rotation);
            inline void setCollsionType(int t){ collisionType_ = t; }
            inline void SetVelocity(const glm::vec3& velocity) { velocity_ = velocity; }

        protected:
            // Object's Transform Variables
            // TODO: Add more transformation variables
            glm::vec3 position_;
            float scale_;
            float rotation_;
            glm::vec3 velocity_;
            bool deceased_;
            float health_;

            int collisionType_;

            std::vector<GameObject*> subObjects;
            GameObject* parent;


            // Object's texture reference
            GLuint texture_;
            bool fill;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
