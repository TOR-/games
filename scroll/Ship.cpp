#include "AnimSpriteComponent.h"
#include "Game.h"
#include "Ship.h"

Ship::Ship(Game* game) : Actor(game), _rightSpeed(0.0f), _downSpeed(0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> antex = {
		game->GetTexture("Ship01.png"),
		game->GetTexture("Ship02.png"),
		game->GetTexture("Ship03.png"),
		game->GetTexture("Ship04.png")
	};
	asc->SetAnimTextures(antex);
}

void Ship::UpdateActor(float dt)
{
	Actor::UpdateActor(dt);
	Vector2 pos = get_position();
	pos.x += _rightSpeed * dt;
	pos.y += _downSpeed  * dt;
	if (pos.x < 35.0f)
		pos.x = 35.0f;
	else if (pos.x > Game::WIDTH - 50.0f)
		pos.x = Game::WIDTH - 50.0f;
	if (pos.y < 25.0f)
		pos.y = 25.0f;
	else if (pos.y > Game::HEIGHT)
		pos.y = Game::HEIGHT;
	set_position(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state)
{
	_rightSpeed = 0.0f;
	_downSpeed  = 0.0f;
	if (state[SDL_SCANCODE_D])
		_rightSpeed += HSPEED;
	if (state[SDL_SCANCODE_A])
		_rightSpeed -= HSPEED;
	if (state[SDL_SCANCODE_S])
		_downSpeed  += VSPEED;
	if (state[SDL_SCANCODE_W])
		_downSpeed  -= VSPEED;
}
