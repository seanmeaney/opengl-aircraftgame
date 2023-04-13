#include "HUD.h"

HUD::HUD(GLuint texture, GLuint texture2, GLuint texture3, GLuint texture4, GLuint texture5){
    rocket_body_ = new GameObject(glm::vec3(-5.0f,1.0f,-0.01f),texture);
    rocket_body_->SetScale(0.5);
    body = false;

    rocket_booster1_ = new GameObject(glm::vec3(-5.0f,0.5f,-0.01f),texture2);
    rocket_booster1_->SetScale(0.5);
    booster1 =false;

    rocket_booster2_ = new GameObject(glm::vec3(-5.0f,0.0f,-0.01f),texture2);
    rocket_booster2_->SetScale(0.5);
    booster2 = false;

    rocket_fuel_ = new GameObject(glm::vec3(-5.0f,-0.5f,-0.01f),texture3);
    rocket_fuel_->SetScale(0.5);
    fuel = false;

    missile_ = new GameObject(glm::vec3(-5.0f,-1.0f,-0.01f),texture4);
    missile_->SetScale(0.5);
    missile = false;

    background = new GameObject(glm::vec3(-9.0f,1.0f,-0.01f),texture5);
    background->SetScale(10.0f);
}


void HUD::render(Shader& sprite_shader, double current_time){
    if (body){
        rocket_body_->Render(sprite_shader, current_time);
    }
    
    if(booster1){
        rocket_booster1_->Render(sprite_shader, current_time);
    }
    if(booster2){
        rocket_booster2_->Render(sprite_shader, current_time);
    }
    if(fuel){
        rocket_fuel_->Render(sprite_shader, current_time);
    }
    if(missile){
        missile_->Render(sprite_shader, current_time);
    }

    background->Render(sprite_shader, current_time);
}