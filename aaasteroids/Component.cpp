#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int update_order)
	: _owner(owner), _updateOrder(update_order)
{
	_owner->AddComponent(this);
}

Component::~Component()
{
	_owner->RemoveComponent(this);
}

void Component::Update(float dt) {}
