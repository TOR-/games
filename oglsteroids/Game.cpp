#include <algorithm>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL_image.h>

#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"
#include "Math.h"

Game::Game(const char * assetpath): _assetpath(assetpath), _window(nullptr), _isRunning(true), _updatingActors(false) {};

bool Game::Initialise()
{
	int sdl_result = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	if (sdl_result != 0)
	{
		SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK , SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE             , 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE           , 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE            , 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE           , 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER         , 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL   , 1);
	_window = SDL_CreateWindow( "AAASTEROIDS", 100, 100, WIDTH, HEIGHT
			, SDL_WINDOW_OPENGL);
	if (!_window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	_context = SDL_GL_CreateContext(_window);
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		SDL_Log("Failed to init GLEW. Exiting...");
		return false;
	}
	glGetError();

	if(!LoadShaders())
	{
		SDL_Log("Failed to load shaders. Exiting...");
		return false;
	}

	InitSpriteVerts();
	LoadData();

	_ticksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
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
	glClearColor(0.75f, 0.86f, 0.76f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	_spriteShader->SetActive();
	_spriteVerts->SetActive();

	for(auto s:_sprites)
		s->Draw(_spriteShader);

	SDL_GL_SwapWindow(_window);
}

void Game::LoadData()
{
	_ship = new Ship(this);
	_ship->set_position(Vector2(WIDTH/2.0f, HEIGHT/2.0f));
	_ship->set_scale(1.0f);

	// Background
	//Actor* bg_actor = new Actor(this);
	//bg_actor->set_position(Vector2(WIDTH/2.0f, HEIGHT/2.0f));
	
	//for (int i = 0; i < init_asteroids; i++)
	//	AddAsteroid(new Asteroid(this));
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

		tex = 0;//SDL_CreateTextureFromSurface(_renderer, surf);
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

void Game::AddAsteroid(Asteroid* a)
{
	_asteroids.emplace_back(a);
}

void Game::RemoveAsteroid(Asteroid* a)
{
	auto ai = std::find(_asteroids.begin(), _asteroids.end(), a);
	if(ai!=_asteroids.end()) _asteroids.erase(ai);
}

void Game::InitSpriteVerts()
{
	float vtxBuf[] = {
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};
	unsigned idxBuf[] = {
		0, 1, 2,
		2, 3, 0
	};
	_spriteVerts = new VertexArray(vtxBuf, 4, idxBuf, 6);
}

bool Game::LoadShaders()
{
	_spriteShader = new Shader();
	if(!_spriteShader->Load("oglsteroids/shaders/basic.vert", "oglsteroids/shaders/basic.frag"))
		return false;
	_spriteShader->SetActive();
	return true;
}
