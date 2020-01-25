#pragma once

#include "Actor.h"
#include "CircleComponent.h"

class Laser : public Actor
{
	public:
		Laser(class Game* game);
		void UpdateActor (float dt) override;

		class CircleComponent* get_bounds() {return _bounds;}
	private:
		const float LASER_RADIUS = 10.0f;
		class CircleComponent* _bounds;
};
