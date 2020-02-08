#include "MoveComponent.h"

MoveComponent::MoveComponent(class Actor * owner, int updateOrder):
	Component(owner, updateOrder), _angularSpeed(0.0f), _forwardSpeed(0.0f)
{}

void MoveComponent::Update(float dt)
{
	if (!Math::near_zero(_angularSpeed))
	{
		float rot = _owner->get_rotation();
		rot += _angularSpeed * dt;
		_owner->set_rotation(rot);
	}
	if (!Math::near_zero(_forwardSpeed))
	{
		Vector2 pos = _owner->get_position();
		pos += _owner->get_forward() * _forwardSpeed * dt;
		_owner->set_position(pos);
	}
}
