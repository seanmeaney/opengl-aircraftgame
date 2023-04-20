#ifndef CURSOR_GAME_OBJECT_H
#define CURSOR_GAME_OBJECT_H

#include "game_object.h"

namespace game
{

	class CursorGameObject : public GameObject
	{

	public:
		CursorGameObject(const glm::vec3 &position, GLuint texture, GLuint texture2, GameObject *player);

		inline void setLock(GameObject *l) {
			target_ = l;
			if(l){
				texture_ = other_tex;
			} else {
				texture_ = default_tex;
			}
		}

		inline GameObject* getTarget(void) { return target_; }

		void mouseCallback(double, double);

		bool Collide(GameObject *) override;

	private:
		GameObject *player_;
		GameObject *target_;

		GLuint default_tex;
		GLuint other_tex;
	};

}

#endif