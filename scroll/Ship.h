#pragma once

#include "Actor.h"

class Ship : public Actor
{
	public:
		Ship(class Game * game);
		void UpdateActor(float dt) override;
		void ProcessKeyboard(const uint8_t * state);
		float get_rightSpeed() const {return _rightSpeed; }
		float get_downSpeed() const {return _downSpeed; }
	private:
		const float HSPEED = 250.0f;
		const float VSPEED = 300.0f;
		float _rightSpeed;
		float _downSpeed;
};
