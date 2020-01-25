#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

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
		static const int WIDTH = 1024;
		static const int HEIGHT = 576;
		static const int INIT_ASTEROID_SPEED = 150.0f;
		static const int n_asteroids = 20;
	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();
		void LoadData();
		void UnloadData();

		// loaded textures //
		std::unordered_map<std::string, SDL_Texture*> _textures;

		std::vector<class Actor*> _actors;
		std::vector<class Actor*> _pendingActors;

		std::vector<class SpriteComponent*> _sprites;

		const char * _assetpath;
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		bool _isRunning;
		uint32_t _ticksCount;
		bool _updatingActors;

		//=> GAME-SPECIFIC <=//
		class Ship* _ship;
};
#endif
