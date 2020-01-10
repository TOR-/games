#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <cstdint>

struct vec2
{
	float x;
	float y;
};

enum PaddleDir
{
	UP = -1, NONE = 0, DOWN = 1
};

class Game
{
	public:
		Game();
		bool Initialise();
		void RunLoop();
		void Shutdown();

		static const int WIDTH = 800;
		static const int HEIGHT = 600;
		static const int WALL_THICKNESS = 20;
		static const int PADDLE_THICKNESS = WALL_THICKNESS;
		static const int PADDLE_HEIGHT = 6*PADDLE_THICKNESS;
		static const int BALL_THICKNESS = WALL_THICKNESS;

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		SDL_Window* _Window;
		SDL_Renderer* _Renderer;
		bool _IsRunning;
		uint32_t _TicksCount;
		struct vec2 _pos_ball;
		struct vec2 _vel_ball;
		struct vec2 _pos_pad;
		PaddleDir _paddle_dir;
};
#endif
