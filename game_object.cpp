#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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
    collisionType_ = 1;
    deceased_ = false;
    parent = nullptr;
    health_ = 100.0f;
}


bool GameObject::Collide(GameObject *other){
    if(!deceased_ && !other->isKill() &&  collisionType_ == 1 && other->collisionType_ == 1){
        if(glm::distance(position_, other->position_) < 0.5f){
            deceased_ = true;
            other->deceased_ = true;
            return true;
        }
    } else if (other->collisionType_ == 2 || other->collisionType_ == 3 || other->collisionType_ == 4){
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

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    if (parent){
        glm::mat4 p_rotation_matrix = glm::rotate(glm::mat4(1.0f), parent->GetAngle(), glm::vec3(0.0, 0.0, 1.0));

        glm::mat4 p_translation_matrix = glm::translate(glm::mat4(1.0f), parent->GetPosition());

        glm::mat4 p_transformation_matrix = p_translation_matrix * p_rotation_matrix;

        transformation_matrix = p_transformation_matrix * transformation_matrix;
    }

    // Set the transformation matrix in the shader
    shader.SetUniformMat4("transformation_matrix", transformation_matrix);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, shader.GetSpriteSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
