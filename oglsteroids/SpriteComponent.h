#pragma once

#include "Component.h"
#include <SDL2/SDL.h>

class SpriteComponent : public Component
{
	public:
		SpriteComponent(class Actor * owner, int draw_order = 100);
		~SpriteComponent();

		virtual void Draw(SDL_Renderer* renderer);
		virtual void SetTexture(SDL_Texture* texture);

		int get_drawOrder() const {return _drawOrder;}
		int get_texWidth() const {return _texWidth;}
		int get_texHeight() const {return _texHeight;}
	protected:
		SDL_Texture* _texture;
		int _drawOrder;
		int _texWidth;
		int _texHeight;
};
