#include "Asteroid.h"
#include "Game.h"
#include "Math.h"
#include "MoveComponent.h"
#include "Random.h"
#include "SpriteComponent.h"

Asteroid::Asteroid(Game * game):Actor(game)
{
	Vector2 startPos = Random::GetVector(Vector2::ZERO, Vector2(Game::WIDTH, Game::HEIGHT));
	set_position(startPos);
	set_rotation(Random::GetFloat(0.0f, Math::TWO_PI));

	SpriteComponent *sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Asteroid.png"));

	MoveComponent* wmc = new MoveComponent(this);
	wmc->set_forwardSpeed(Game::INIT_ASTEROID_SPEED);
}

Asteroid::~Asteroid()
{
	//get_game()->RemoveAsteroid(this);
}

void Asteroid::UpdateActor(float dt)
{
	Vector2 pos = get_position();
	if(pos.x > Game::WIDTH + ASTEROID_WIDTH)
		pos.x = -ASTEROID_WIDTH;
	else if(pos.x < -ASTEROID_WIDTH)
		pos.x = Game::WIDTH + ASTEROID_WIDTH;
	if(pos.y > Game::HEIGHT + ASTEROID_WIDTH)
		pos.y = -ASTEROID_WIDTH;
	else if(pos.y < -ASTEROID_WIDTH)
		pos.y = Game::HEIGHT + ASTEROID_WIDTH;
	set_position(pos);
}
