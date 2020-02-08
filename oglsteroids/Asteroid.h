#pragma once

#include "Actor.h"
#include "CircleComponent.h"

class Asteroid : public Actor
{
	public:
		Asteroid(class Game* game);
		~Asteroid();
		void UpdateActor (float dt) override;

		class CircleComponent* get_bounds() {return _bounds;}
	private:
		const float ASTEROID_RADIUS = 20.0f;
		class CircleComponent* _bounds;
};
