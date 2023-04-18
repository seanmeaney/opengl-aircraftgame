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
        // float camera_zoom = 0.25f;

        double xpos, ypos;
        glfwGetCursorPos(window_, &xpos, &ypos);

        //convert cursor position from window position to game coordinates
        float aspect_ratio = (float)window_width_g/(float)window_height_g;
        position_.x = ((2.0f*xpos - window_width_g)* aspect_ratio)/(window_width_g*camera_zoom) + player_->GetPosition().x;
        position_.y = (-2.0f*ypos + window_height_g)/(window_height_g*camera_zoom) + player_->GetPosition().y;


        // GameObject::Update(delta_time);
    }
}