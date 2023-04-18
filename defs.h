#ifndef DEFS_H
#define DEFS_H

//GAME OBJECT CONSTANTS
const float PLAYER_SPEED_MOVE = 0.03;
const float PLAYER_SPEED_ROTATE = 0.02;
const float ENEMY_SPEED_MOVE = 0.5;
const float ENEMY_TARGET_DISTANCE = 4;
const float BULLET_SPEED = 5.0;
const float MISSILE_SPEED = 2.0;


//GAME CONSTANTS
const unsigned int window_width_g = 800*2;
const unsigned int window_height_g =  600*2;
const float camera_zoom = 0.25f;  

const int GAME_MIN_NUM_ENEMIES = 50;
const int GAME_MAX_NUM_ENEMIES = 75;
const int GAME_ENEMY_SPAWN_RADIUS = 40;

const float GAME_ENDGAME_TIME = 4.0;


//GAME ENUMS AND STRUCTS
struct Inventory {
    int numMissiles = 0;
    bool rocketBody = false;
    int rocketBooster = 0;
    bool rocketFuel = false;
};

enum TextureNums{
    TEX_CHOPPER = 0,
    TEX_REDPLANE = 1,
    TEX_GREENPLANE = 2,
    TEX_DUNE = 3,
    TEX_ORB = 4,
    TEX_BLADE = 5,
    TEX_BULLET = 6,
    TEX_CURSOR = 7,
    TEX_CURSOR_LOCK = 8,
    TEX_MISSILE = 9,
    TEX_ROCKET = 10,
    TEX_GAS = 11,
    TEX_ROCKET_BODY = 12,
    TEX_BOOSTER = 13,
    TEX_HUD = 14
};

enum CollisionTypes{
    NOCOLLISION = 0, 
    STANDARDCOLLISION = 1, 
    BULLETCOLLISION = 2,
    MISSILECOLLISION = 3, 
    CURSORCOLLISION = 4
};

enum ObjTypes{ MissileType, RocketBodyType, RocketBoosterType, RocketFuel};


#endif