#include <glm/gtc/matrix_transform.hpp>

#include "game_object.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, GLuint texture) 
{

    // Initialize all attributes
    position_ = position;
    scale_ = 1.0;
    rotation_ = 0.0;
    velocity_ = glm::vec3(0.0f, 0.0f, 0.0f); // Starts out stationary
    texture_ = texture;
    fill = true;
    collisionType_ = STANDARDCOLLISION;
    deceased_ = false;
    parent = nullptr;
    health_ = DEFUALT_STARTING_HEALTH;
    shadow_ = true;
}


bool GameObject::Collide(GameObject *other){
    if(!deceased_ && !other->isKill() &&  collisionType_ == STANDARDCOLLISION && other->collisionType_ == STANDARDCOLLISION){
        if(glm::distance(position_, other->position_) < 0.5f){
            deceased_ = true;
            other->deceased_ = true;
            return true;
        }
    } else if (other->collisionType_ == BULLETCOLLISION || other->collisionType_ == MISSILECOLLISION || other->collisionType_ == CURSORCOLLISION){
        other->Collide(this);
    }
    return false;
}


glm::vec3 GameObject::GetBearing(void) {

    // Assumes sprite is initially rotate by 90 degrees
    float pi_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(rotation_ + pi_two), sin(rotation_ + pi_two), 0.0);
    return dir;
}


glm::vec3 GameObject::GetRight(void) {

    // Assumes sprite is initially rotate by 90 degrees
    glm::vec3 dir(cos(rotation_), sin(rotation_), 0.0);
    return dir;
}


void GameObject::SetAngle(float angle){ 
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    rotation_ = angle;
}


void GameObject::Update(double delta_time) {

    for (int i = 0; i < subObjects.size(); i++)
    {
        subObjects[i]->Update(delta_time);
    }
    

    if(health_ <= 0.0f){
        deceased_ = true;
    }

    // Update object position with Euler integration
    position_ += velocity_ * ((float) delta_time);
}


void GameObject::Render(Shader &shader, double current_time){

    for (int i = 0; i < subObjects.size(); i++){
        subObjects[0]->Render(shader, current_time);
    }
    
    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Set up the shader
    shader.Enable();
    shader.SetSpriteAttributes();

    shader.SetUniform1f("num_tiles", scale_);
    shader.SetUniform1i("fill", fill);
    shader.SetUniform1i("shadow", false);

    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0));

    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0, 0.0, 1.0));

    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    glm::vec3 shadowPos = position_;
    shadowPos.x -= shadow_offset;
    shadowPos.y -= shadow_offset;
    glm::mat4 shadow_translation_matrix = glm::translate(glm::mat4(1.0f), shadowPos);

    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;
    glm::mat4 shadow_transformation_matrix = shadow_translation_matrix * rotation_matrix * scaling_matrix;

    if (parent){
        glm::mat4 p_rotation_matrix = glm::rotate(glm::mat4(1.0f), parent->GetAngle(), glm::vec3(0.0, 0.0, 1.0));

        glm::mat4 p_translation_matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition());
        
        glm::mat4 p_shadow_translation_matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition());

        glm::mat4 p_transformation_matrix = p_translation_matrix * p_rotation_matrix;
        glm::mat4 p_shadow_transformation_matrix = p_shadow_translation_matrix;

        transformation_matrix = p_transformation_matrix * transformation_matrix;
        shadow_transformation_matrix = p_shadow_transformation_matrix * shadow_transformation_matrix;
    }
    shader.SetUniformMat4("transformation_matrix", transformation_matrix);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, shader.GetSpriteSize(), GL_UNSIGNED_INT, 0);


    if (shadow_){
        shader.SetUniform1i("shadow", true);
        shader.SetUniformMat4("transformation_matrix", shadow_transformation_matrix);
        glDrawElements(GL_TRIANGLES, shader.GetSpriteSize(), GL_UNSIGNED_INT, 0);
    }
}

} // namespace game
