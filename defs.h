#ifndef DEFS_H
#define DEFS_H

//SPEED CONSTANTS
const float PLAYER_SPEED_MOVE = 0.03;
const float PLAYER_SPEED_ROTATE = 0.02;
const float ENEMY_SPEED_MOVE = 0.5;
const float BULLET_SPEED = 5.0;

//GAME CONSTANTS
const unsigned int window_width_g = 800*2;
const unsigned int window_height_g =  600*2;  

const float GAME_ENDGAME_TIME = 4.0;



//GAME OBJECT SCALING CONSTANTS

// const double BULLET_SIZE = 0.3;
// const double MISSILE_SCALE = 0.4;
// const double PARTICTLE_SCALE = 0.2;

enum ObjTypes{ MissileType, RocketBodyType, RocketBoosterType, RocketFuel};


#endif