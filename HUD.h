#ifndef HUD_H
#define HUD_H

#include "game_object.h"

using namespace game;

class HUD {
		
	public:
		HUD(GLuint texture, GLuint texture2, GLuint texture3, GLuint texture4, GLuint texture5);
		
		void update(Inventory &);

		inline void showBody(bool b) {body  = b; }
		inline void showB1(bool b) {booster1  = b; }
		inline void showb2(bool b) {booster2  = b; }
		inline void showFuel(bool b) {fuel  = b; }
		inline void showMissile(bool b) {missile  = b; }

		void render(Shader& sprite_shader, double current_time);
	
	private:
		GameObject *rocket_body_;
		bool body;
		GameObject *rocket_booster1_;
		bool booster1;
		GameObject *rocket_booster2_;
		bool booster2;
		GameObject *rocket_fuel_;
		bool fuel;
		GameObject *missile_;
		bool missile;

		GameObject *background;
	
};
#endif