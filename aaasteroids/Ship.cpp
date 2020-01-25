#include <cstdio>
#include <SDL2/SDL.h>

#include "AnimSpriteComponent.h"
#include "Asteroid.h"
#include "CircleComponent.h"
#include "Game.h"
#include "InputComponent.h"
#include "Laser.h"
#include "Math.h"
#include "Ship.h"

Ship::Ship(Game* game) : Actor(game), _isRespawning(false), _deaths(0)
{
	

	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> antex = {
		game->GetTexture("Ship01.png"),
		game->GetTexture("Ship02.png"),
		game->GetTexture("Ship03.png"),
		game->GetTexture("Ship04.png")
	};
	asc->SetAnimTextures(antex);

	InputComponent* ic = new InputComponent(this,
			SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D,
			Game::SHIP_FORE_SPEED, Game::SHIP_TURN_SPEED);
	_bounds = new CircleComponent(this);
	_bounds->set_radius(30.0f);	
}

void Ship::ActorInput(const uint8_t * keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && _laserCooldown <= 0.0f)
	{
		Laser* laser = new Laser(get_game());
		// match self pos & rot
		laser->set_position(get_position());
		laser->set_rotation(get_rotation());

		_laserCooldown = Game::LASER_COOLDOWN;
	}
}

void Ship::UpdateActor(float dt)
{
	_laserCooldown -= dt;
/*	if(true == _isRespawning)
	{
		_respawnCooldown -= dt;
		if(_respawnCooldown <= 0)
		{
			set_state(State::Active);
			_isRespawning = false;
		}
	}
	else
	{
	*/
	for (auto asteroid : get_game()->get_asteroids())
		if( Intersect(*_bounds, *(asteroid->get_bounds())))
		{
			_deaths++;
			set_position(Vector2(Game::WIDTH / 2.0f, Game::HEIGHT / 2.0f));
			printf("Fuair tu bás %u uair.\n", _deaths);
			/*
			   set_state(State::Dead);
			   _isRespawning = true;
			   _respawnCooldown = Game::LASER_COOLDOWN;
			   */
			break;
		}
}
