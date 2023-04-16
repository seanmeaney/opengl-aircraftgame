#ifndef DEFS_H
#define DEFS_H

//SPEED CONSTANTS
const float PLAYER_SPEED_MOVE = 0.03;
const float PLAYER_SPEED_ROTATE = 0.02;
const float ENEMY_SPEED_MOVE = 0.5;
const float BULLET_SPEED = 5.0;
const float MISSILE_SPEED = 2.0;


//GAME CONSTANTS
const unsigned int window_width_g = 800*2;
const unsigned int window_height_g =  600*2;  

const int GAME_MIN_NUM_ENEMIES = 50;
const int GAME_MAX_NUM_ENEMIES = 75;
const int GAME_ENEMY_SPAWN_RADIUS = 40;

const float GAME_ENDGAME_TIME = 4.0;



//GAME OBJECT SCALING CONSTANTS

// const double BULLET_SIZE = 0.3;
// const double MISSILE_SCALE = 0.4;
// const double PARTICTLE_SCALE = 0.2;

//GAME ENUMS

enum CollisionTypes{
    NOCOLLISION = 0, 
    STANDARDCOLLISION = 1, 
    BULLETCOLLISION = 2,
    MISSILECOLLISION = 3, 
    CURSORCOLLISION = 4
};

enum ObjTypes{ MissileType, RocketBodyType, RocketBoosterType, RocketFuel};


#endif