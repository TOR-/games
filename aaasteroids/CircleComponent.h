#pragma once

#include "Actor.h"
#include "Component.h"
#include "Math.h"

class CircleComponent: public Component
{
	public:
		CircleComponent(class Actor* owner):Component(owner){};

		void set_radius(float r) { _radius = r; }
		float get_radius() const { return _radius; }

		const Vector2& get_center() const {return _owner->get_position();};
	
	private:
		float _radius;
};

bool Intersect(const CircleComponent& ca, const CircleComponent& cb);
