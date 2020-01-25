#include "Asteroid.h"
#include "CircleComponent.h"
#include "Laser.h"
#include "Game.h"
#include "Math.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"

Laser::Laser(Game * game):Actor(game)
{
	SpriteComponent *sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Laser.png"));

	MoveComponent* mc = new MoveComponent(this);
	mc->set_forwardSpeed(Game::LASER_SPEED);

	_bounds = new CircleComponent(this);
	_bounds->set_radius(LASER_RADIUS);

}

void Laser::UpdateActor(float dt)
{
	for (auto asteroid : get_game()->get_asteroids())
		if( Intersect(*_bounds, *(asteroid->get_bounds())))
		{
			set_state(State::Dead);
			asteroid->set_state(State::Dead);
			break;
		}
}
