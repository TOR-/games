#pragma once

#include <vector>

#include "SpriteComponent.h"

class AnimSpriteComponent : public SpriteComponent
{
	public:
		AnimSpriteComponent(class Actor* owner, int draw_order = 100);
		void Update(float dt) override;
		// Set textures
		void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
		float get_animFPS() const {return _animFPS;}
		void set_animFPS(float fps) { _animFPS = fps;}
	private:
		std::vector<SDL_Texture*> _animTextures;
		// Frame currently displayed
		float _currentFrame;
		float _animFPS;
};
