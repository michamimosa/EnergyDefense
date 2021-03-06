#ifndef _PLAYER_H
#define _PLAYER_H

#include <oxygarum.h>
#include "chunk.h"

using namespace oxygarum;

class Player
{
	public:
		Player();
		~Player();

		void update(float speed, Chunk **chunks);
		void move(float vel);
		void jump(float vel);

		Mesh3D *mesh;
		Object3D *obj;

		int candies;

	private:
		float vel_x;
		float vel_y;
};

#endif

