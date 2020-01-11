#pragma once

#include <vector>

#include "Math.h"

class Actor
{
	public:
		enum State
		{
			Active,
			Paused,
			Dead
		};
		Actor(class Game* game);
		virtual ~Actor();

		// Update fn called within Game
		void Update(float dt);
		void UpdateComponents(float dt);
		// Actor specific update code
		virtual void UpdateActor(float dt);

		State get_state() const {return _state;}
		void set_state(State s) {_state = s;}
		
		const Vector2& get_position() const {return _position;}
		void set_position(const Vector2& p) {_position = p;}
		
		float get_scale() const {return _scale;}
		void set_scale(float s) {_scale = s;}
		
		float get_rotation() const {return _rotation;}
		void set_rotation(float r) {_rotation = r;}

		class Game* get_game() { return _game; }

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);
	private:
		State _state;
		Vector2 _position;
		float _scale;    // 1.0f ≡ 100%
		float _rotation; // radians
		std::vector<class Component *> _components;
		class Game* _game;
};
