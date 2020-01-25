#pragma once

#include "Actor.h"

class Asteroid : public Actor
{
	public:
		Asteroid(class Game* game);
		~Asteroid();
		void UpdateActor (float dt) override;
	private:
		const float ASTEROID_WIDTH = 20.0f;
};
