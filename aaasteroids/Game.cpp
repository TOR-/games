#include <algorithm>
#include <SDL2/SDL_image.h>

#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"
#include "Math.h"
#include "BGSpriteComponent.h"

Game::Game(const char * assetpath): _assetpath(assetpath), _window(nullptr), _renderer(nullptr), _isRunning(true), _updatingActors(false) {};

bool Game::Initialise()
{
	int sdl_result = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	if (sdl_result != 0)
	{
		SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
		return false;
	}

	_window = SDL_CreateWindow( "AAASTEROIDS", 100, 100, WIDTH, HEIGHT, 0 );
	if (!_window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	_renderer = SDL_CreateRenderer( _window, -1
			, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if (!_renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialise SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	_ticksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (_isRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::ProcessInput(){
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				_isRunning = false;
				break;
		}
	}
	
	const uint8_t * state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
		_isRunning = false;

	//_ship->ProcessKeyboard(state);
	_updatingActors = true;
	for(auto actor : _actors) actor->ProcessInput(state);
	_updatingActors = false;
}

void Game::UpdateGame()
{
	// Wait 16 ms elapsed from last frame
	// (For target 60 FPS)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), _ticksCount + 16));

	float dt = (SDL_GetTicks() - _ticksCount) / 1000.0f;

	// Clamp max dt
	dt = dt > 0.05f ? 0.05f : dt;
	_ticksCount = SDL_GetTicks();

	// UPDATE GAME OBJECTS AS F(DT)
	
	_updatingActors = true;
	for (auto actor: _actors)
		actor->Update(dt);
	_updatingActors = false;
	
	for (auto pending : _pendingActors)
		_actors.emplace_back(pending);
	_pendingActors.clear();

	// Removing dead actors
	std::vector<Actor*> deadActors;
	for (auto a: _actors)
		if (a->get_state() == Actor::Dead)
			deadActors.emplace_back(a);
	for (auto a: deadActors)
		delete a;
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
	
	// DRAW ALL SPRITES
	for (auto s: _sprites)
		s->Draw(_renderer);
	
	SDL_RenderPresent(_renderer);
}

void Game::LoadData()
{
	_ship = new Ship(this);
	_ship->set_position(Vector2(100.0f, HEIGHT/2.0f));
	_ship->set_scale(1.5f);

	// Background
	Actor* bg_actor = new Actor(this);
	bg_actor->set_position(Vector2(WIDTH/2.0f, HEIGHT/2.0f));
	
	BGSpriteComponent* bg = new BGSpriteComponent(bg_actor);
	bg->set_screenSize(Vector2(WIDTH, HEIGHT));
	std::vector<SDL_Texture*> bg_tex = { 
		GetTexture("Farback01.png"),
		GetTexture("Farback02.png")
	};
	bg->SetBGTextures(bg_tex);
	bg->set_scrollSpeed(-100.0f);

	bg = new BGSpriteComponent(bg_actor, 50);
	bg->set_screenSize(Vector2(WIDTH, HEIGHT));
	bg_tex = { 
		GetTexture("Stars.png"),
		GetTexture("Stars.png") 
	};
	bg->SetBGTextures(bg_tex);
	bg->set_scrollSpeed(-200.0f);

	for (int i = 0; i < n_asteroids; i++)
		new Asteroid(this);
}

void Game::UnloadData()
{
	while (!_actors.empty())
		delete _actors.back();

	for (auto t: _textures)
		SDL_DestroyTexture(t.second);
	_textures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& filename)
{
	std::string filepath(_assetpath);
	filepath.append("/").append(filename);
	SDL_Texture* tex = nullptr;
	auto ti = _textures.find(filename);
	if (ti != _textures.end())
	{
		// In map
		tex = ti->second;
	}
	else
	{	// Must load texture
		SDL_Surface* surf = IMG_Load(filepath.c_str());
		if(!surf)
		{
			SDL_Log("Failed to load texture from file %s: %s", filepath.c_str(), SDL_GetError());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(_renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to create texture from surface for %s: %s",
					filepath.c_str(), SDL_GetError());
			return nullptr;
		}

		_textures.emplace(filename.c_str(), tex);
	}
	return tex;
}

void Game::AddActor(Actor* actor)
{
	if (_updatingActors)
		_pendingActors.emplace_back(actor);
	else
		_actors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto i = std::find(_pendingActors.begin(), _pendingActors.end(), actor);
	if (i != _pendingActors.end())
	{
		std::iter_swap(i, _pendingActors.end() - 1);
		_pendingActors.pop_back();
	}

	i = std::find(_actors.begin(), _actors.end(), actor);
	if (i != _actors.end())
	{
		std::iter_swap(i, _actors.end() - 1);
		_actors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// insert before element with higher draw order
	int drawOrder = sprite->get_drawOrder();
	auto i = _sprites.begin();
	for( ; i != _sprites.end(); ++i)
		if (drawOrder < (*i)->get_drawOrder())
			break;
	_sprites.insert(i, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// Swapping would ruin the ordering
	auto i = std::find(_sprites.begin(), _sprites.end(), sprite);
	_sprites.erase(i);
}
