#pragma once

#include "Actor.h"

class Ship : public Actor
{
	public:
		Ship(class Game * game);
		void UpdateActor (float dt) override;
		void ActorInput (const uint8_t* keyState) override;

		class CircleComponent* get_bounds() {return _bounds;}
	private:
		float _laserCooldown;
		float _respawnCooldown;
		bool _isRespawning;
		unsigned _deaths;
		class CircleComponent* _bounds;
};
