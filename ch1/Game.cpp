#include <iostream>
#include <random>
#include <SDL2/SDL.h>

#include "Game.h"

Game::Game(int nplayers, int nballs): _nplayers(nplayers), _nballs(nballs), _Window(nullptr), _IsRunning(true) {};

bool Game::Initialise()
{
	int sdl_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_result != 0)
	{
		SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
		return false;
	}

	_Window = SDL_CreateWindow( "PONG ONE PLAYER", 100, 100, WIDTH, HEIGHT, 0 /* SDL_WINDOW_FULLSCREEN */);
	if (!_Window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	_Renderer = SDL_CreateRenderer( _Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if (!_Renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	_pos_ball = { WIDTH / 2, HEIGHT / 2 };
	_pos_pad_l = { 2*PADDLE_THICKNESS , HEIGHT / 2 };
	_pos_pad_r = { WIDTH - 2*PADDLE_THICKNESS , HEIGHT / 2 };

	std::default_random_engine gen;
	std::uniform_real_distribution<float> dist_y(70,100);
	std::uniform_real_distribution<float> dist_x(90,100);
	_vel_ball = {dist_x(gen), dist_y(gen)};

	return true;
}

void Game::Shutdown()
{
	SDL_DestroyWindow(_Window);
	SDL_DestroyRenderer(_Renderer);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (_IsRunning)
	{
		ProcessInput();
		UpdateGame();
		if (_pos_ball.x < 0)
		{
			std::cout << "Cháil Imreoir a hAon." << std::endl;
			_IsRunning = false;
		}	
		else if (_pos_ball.x > WIDTH)
		{
			std::cout << "Cháil Imreoir a Dó." << std::endl;
			_IsRunning = false;
		}
			
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
				_IsRunning = false;
				break;
		}
	}
	
	const uint8_t * state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
		_IsRunning = false;
	_paddle_dir_l = NONE;
	if ( state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S])
		_paddle_dir_l = UP;
	if (!state[SDL_SCANCODE_W] &&  state[SDL_SCANCODE_S])
		_paddle_dir_l = DOWN;
	_paddle_dir_r = NONE;
	if ( state[SDL_SCANCODE_I] && !state[SDL_SCANCODE_K])
		_paddle_dir_r = UP;
	if (!state[SDL_SCANCODE_I] &&  state[SDL_SCANCODE_K])
		_paddle_dir_r = DOWN;
}

void Game::UpdateGame()
{
	// Wait 16 ms elapsed from last frame
	// (For target 60 FPS
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), _TicksCount + 16));

	float dt = (SDL_GetTicks() - _TicksCount) / 1000.0f;

	// Clamp max dt
	dt = dt > 0.05f ? 0.05f : dt;
	// Update game objects as f(dt)
	if (_paddle_dir_l != 0)
	{
		_pos_pad_l.y += static_cast<int>(_paddle_dir_l) * PADDLE_SPEED * dt;
		if (_pos_pad_l.y < (PADDLE_HEIGHT/2.0f + WALL_THICKNESS))
			_pos_pad_l.y = PADDLE_HEIGHT/2.0f + WALL_THICKNESS;
		else if (_pos_pad_l.y > (HEIGHT - PADDLE_HEIGHT/2.0f - WALL_THICKNESS))
			_pos_pad_l.y = HEIGHT - PADDLE_HEIGHT/2.0f - WALL_THICKNESS;
	}
	if( 2 == _nplayers )
		if (_paddle_dir_r != 0)
		{
			_pos_pad_r.y += static_cast<int>(_paddle_dir_r) * PADDLE_SPEED * dt;
			if (_pos_pad_r.y < (PADDLE_HEIGHT/2.0f + WALL_THICKNESS))
				_pos_pad_r.y = PADDLE_HEIGHT/2.0f + WALL_THICKNESS;
			else if (_pos_pad_r.y > (HEIGHT - PADDLE_HEIGHT/2.0f - WALL_THICKNESS))
				_pos_pad_r.y = HEIGHT - PADDLE_HEIGHT/2.0f - WALL_THICKNESS;
		}

	_pos_ball.x += _vel_ball.x *dt;
	_pos_ball.y += _vel_ball.y *dt;

	// Collision
	if ((_pos_ball.y <= WALL_THICKNESS && _vel_ball.y < 0.0f)
			||(_pos_ball.y >= HEIGHT-WALL_THICKNESS && _vel_ball.y > 0.0f)) 
		_vel_ball.y *= -1;
	if (1 == _nplayers && _pos_ball.x >= WIDTH - WALL_THICKNESS && _vel_ball.x > 0.0f)
		_vel_ball.x *= -1;
	if (_pos_ball.y < _pos_pad_l.y + PADDLE_HEIGHT/2.0f && _pos_ball.y > _pos_pad_l.y - PADDLE_HEIGHT/2.0f
			&& _pos_ball.x < _pos_pad_l.x + PADDLE_THICKNESS/2.0f && _pos_ball.x > _pos_pad_l.x - PADDLE_THICKNESS/2.0f
			&& _vel_ball.x < 0.0f)
	{
		if ((_pos_ball.y > _pos_pad_l.y + PADDLE_HEIGHT/4.0f
				   	&& _vel_ball.y > 0.0f)
				||(_pos_ball.y < _pos_pad_l.y - PADDLE_HEIGHT/4.0f 
					&& _vel_ball.y < 0.0f))
			_vel_ball.y *= 1.2f;
		else if ((_pos_ball.y > _pos_pad_l.y + PADDLE_HEIGHT/4.0f
				   	&& _vel_ball.y < 0.0f)
				||(_pos_ball.y < _pos_pad_l.y - PADDLE_HEIGHT/4.0f 
					&& _vel_ball.y > 0.0f))
			_vel_ball.y /= -1.2f;
		else
			_vel_ball.x *= 1.1f;
		_vel_ball.x *= -1;
	}
	if (2 == _nplayers)
	{
		if (_pos_ball.y < _pos_pad_r.y + PADDLE_HEIGHT/2.0f && _pos_ball.y > _pos_pad_r.y - PADDLE_HEIGHT/2.0f
				&& _pos_ball.x > _pos_pad_r.x - PADDLE_THICKNESS/2.0f && _pos_ball.x < _pos_pad_r.x + PADDLE_THICKNESS/2.0f
				&& _vel_ball.x > 0.0f)
		{
			if ((_pos_ball.y > _pos_pad_r.y + PADDLE_HEIGHT/4.0f
						&& _vel_ball.y > 0.0f)
					||(_pos_ball.y < _pos_pad_r.y - PADDLE_HEIGHT/4.0f 
						&& _vel_ball.y < 0.0f))
				_vel_ball.y *= 1.2f;
			else if ((_pos_ball.y > _pos_pad_r.y + PADDLE_HEIGHT/4.0f
						&& _vel_ball.y < 0.0f)
					||(_pos_ball.y < _pos_pad_r.y - PADDLE_HEIGHT/4.0f 
						&& _vel_ball.y > 0.0f))
				_vel_ball.y /= -1.2f;
			else
				_vel_ball.x *= 1.1f;
			_vel_ball.x *= -1;
		}
	}
}

SDL_Rect wall_top   {0, 0, Game::WIDTH, Game::WALL_THICKNESS};
SDL_Rect wall_bot   {0, Game::HEIGHT - Game::WALL_THICKNESS, Game::WIDTH, Game::WALL_THICKNESS};
SDL_Rect wall_right {Game::WIDTH - Game::WALL_THICKNESS, 0, Game::WALL_THICKNESS, Game::HEIGHT};

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(_Renderer, 0, 0, 0, 255);
	SDL_RenderClear(_Renderer);
	
	// Draw scene
	// walls
	SDL_SetRenderDrawColor(_Renderer, 0, 198, 40, 255);
	SDL_RenderFillRect (_Renderer, &wall_top);
	SDL_RenderFillRect (_Renderer, &wall_bot);
	if ( 1 == _nplayers ) SDL_RenderFillRect (_Renderer, &wall_right);
	// Paddles
	SDL_Rect padl{
		static_cast<int>(_pos_pad_l.x - PADDLE_THICKNESS/2),
		static_cast<int>(_pos_pad_l.y - PADDLE_HEIGHT/2),
		PADDLE_THICKNESS, PADDLE_HEIGHT
	};
	SDL_RenderFillRect (_Renderer, &padl);
	if(2 == _nplayers)
	{
		SDL_Rect padr{
			static_cast<int>(_pos_pad_r.x - PADDLE_THICKNESS/2),
				static_cast<int>(_pos_pad_r.y - PADDLE_HEIGHT/2),
				PADDLE_THICKNESS, PADDLE_HEIGHT
		};
		SDL_RenderFillRect (_Renderer, &padr);
	}
	// Ball
	SDL_Rect ball {
		static_cast<int>(_pos_ball.x - BALL_THICKNESS/2),
		static_cast<int>(_pos_ball.y - BALL_THICKNESS/2),
		PADDLE_THICKNESS, PADDLE_THICKNESS
	};
	SDL_RenderFillRect (_Renderer, &ball);
	
	
	SDL_RenderPresent(_Renderer);
}
