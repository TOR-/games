#pragma once

#include "Actor.h"
#include "Component.h"

class MoveComponent : public Component
{
	public:
		// Update this early
		MoveComponent(class Actor* owner, int updateOrder = 10);

		void Update(float dt) override;

		float get_angularSpeed() const {return _angularSpeed;}
		void set_angularSpeed(float speed) { _angularSpeed = speed; }
		float get_forwardSpeed() const {return _forwardSpeed;}
		void set_forwardSpeed(float speed) { _forwardSpeed = speed; }
	private:
		float _angularSpeed;
		float _forwardSpeed;
};
