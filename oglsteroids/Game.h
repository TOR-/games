#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Math.h"
#include "Shader.h"
#include "VertexArray.h"

class Game
{
	public:
		Game(const char * assetpath);
		bool Initialise();
		void RunLoop();
		void Shutdown();

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

		void AddSprite(class SpriteComponent* sprite);
		void RemoveSprite(class SpriteComponent* sprite);

		SDL_Texture* GetTexture(const std::string& file_name);

		void AddAsteroid(class Asteroid* a);
		void RemoveAsteroid(class Asteroid* a);
		std::vector<class Asteroid*>& get_asteroids() { return _asteroids; }
		static const int WIDTH = 1024;//1366
		static const int HEIGHT = 576;//768
		static constexpr float INIT_ASTEROID_SPEED = 150.0f;
		static constexpr float SHIP_FORE_SPEED = 200.0f;
		static constexpr float SHIP_TURN_SPEED = 6.0f;
		static constexpr float SHIP_COOLDOWN = 1.0f;
		static constexpr float LASER_SPEED = 250.0f;
		static constexpr float LASER_COOLDOWN = 0.5f;
		static const int init_asteroids = 30;
	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();
		void LoadData();
		void UnloadData();
		void InitSpriteVerts();
		bool LoadShaders();

		VertexArray* _spriteVerts;
		Shader* _spriteShader;

		// loaded textures //
		std::unordered_map<std::string, SDL_Texture*> _textures;

		std::vector<class Actor*> _actors;
		std::vector<class Actor*> _pendingActors;

		std::vector<class SpriteComponent*> _sprites;

		const char * _assetpath;
		SDL_Window* _window;
		SDL_GLContext _context;
		bool _isRunning;
		uint32_t _ticksCount;
		bool _updatingActors;

		//=> GAME-SPECIFIC <=//
		class Ship* _ship;
		std::vector<class Asteroid*> _asteroids;
};
#endif
