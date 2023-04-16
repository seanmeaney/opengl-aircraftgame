#include "cursor_game_object.h"

namespace game
{
    CursorGameObject::CursorGameObject(const glm::vec3 &position, GLuint texture, GLuint texture2, GLFWwindow *window, GameObject *player)
        : GameObject(position, texture){
        collisionType_ = CURSORCOLLISION;
        window_ = window;
        other_tex = texture2;
        default_tex = texture;
        player_ = player;
        target_ = nullptr;
    }

    bool CursorGameObject::Collide(GameObject *other){
        if(!deceased_ && !other->isKill() && other != player_ && other->getCollisionType() == STANDARDCOLLISION){
            if(glm::distance(position_ * scale_, other->GetPosition() * other->GetScale()) < 0.5f){
                return true;
            }
        }
        return false;
    }


    void CursorGameObject::Update(double delta_time)
    {
        //temp dont do it this way
        //also need viewport info
        float camera_zoom = 0.25f;

        double xpos, ypos;
        glfwGetCursorPos(window_, &xpos, &ypos);

        // Get information about the window
        int width, height;
        glfwGetWindowSize(window_, &width, &height);

        float aspect_ratio = (float)width/(float)height;
        position_.x = ((2.0f*xpos - width)* aspect_ratio)/(width*camera_zoom) + player_->GetPosition().x;
        position_.y = (-2.0f*ypos + height)/(height*camera_zoom) + player_->GetPosition().y;


        // GameObject::Update(delta_time);
    }
}