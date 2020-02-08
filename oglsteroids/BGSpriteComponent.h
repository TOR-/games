#pragma once

#include <vector>

#include "Math.h"
#include "SpriteComponent.h"

class BGSpriteComponent : public SpriteComponent
{
	public:
		BGSpriteComponent(class Actor * owner, int draw_order = 10);
		void Update(float dt) override;
		void Draw(SDL_Renderer* renderer) override;
		void SetBGTextures(const std::vector<SDL_Texture*>& textures);

		void set_screenSize(const Vector2& size) { _screenSize = size; }
		void set_scrollSpeed(float speed) { _scrollSpeed = speed; }
		float get_scrollSpeed() const { return _scrollSpeed; }
	private:
		struct BGTexture
		{
			SDL_Texture* _texture;
			Vector2 _offset;
		};
		std::vector<BGTexture> _BGTextures;
		Vector2 _screenSize;
		float _scrollSpeed;
};
