#include <SDL2/SDL.h>

#include "AnimSpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Math.h"
#include "Ship.h"
#include "Laser.h"

Ship::Ship(Game* game) : Actor(game)
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
}
