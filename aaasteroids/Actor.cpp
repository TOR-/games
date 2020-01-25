#include <algorithm>
#include <stdint.h>

#include "Actor.h"
#include "Component.h"
#include "Game.h"

Actor::Actor(Game * game): _state(Active), _position(Vector2::ZERO), _scale(1.0f)
						   , _rotation(0.0f), _game(game)
{
	_game->AddActor(this);
}
Actor::~Actor()
{
	_game->RemoveActor(this);
	while(!_components.empty())
		delete _components.back();
}

void Actor::Update(float dt)
{
	if (_state == Active)
	{
		UpdateComponents(dt);
		UpdateActor(dt);
	}
}

void Actor::UpdateComponents(float dt)
{
	for (auto c : _components)
	{
		c->Update(dt);
	}
}

void Actor::UpdateActor(float dt){}

void Actor::AddComponent(Component* component)
{
	// Add component at correct location in components
	int upOrder = component->get_updateOrder();
	auto i = _components.begin();
	for(;i != _components.end();++i)
		if (upOrder < (*i)->get_updateOrder())
			break;
	_components.insert(i, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto i = std::find(_components.begin(), _components.end(), component);
	if (i != _components.end())
	{
		_components.erase(i);
	}
}

void Actor::ProcessInput(const uint8_t * keyState)
{
	if(_state == State::Active)
	{
		for(auto c : _components)
			c->ProcessInput(keyState);
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState) {}
