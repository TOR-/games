#include "AnimSpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Math.h"
#include "Ship.h"

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
		   	SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_A,
			300.0f, Math::TWO_PI);
}

