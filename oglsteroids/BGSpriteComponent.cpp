#include "Actor.h"
#include "BGSpriteComponent.h"

BGSpriteComponent::BGSpriteComponent( class Actor* owner, int draw_order)
	: SpriteComponent(owner, draw_order), _scrollSpeed(0.0f) {}

void BGSpriteComponent::Update(float dt)
{
	SpriteComponent::Update(dt);
	for (auto& bg : _BGTextures)
	{
		// Update xoffset, wrapping if offscreen
		bg._offset.x += _scrollSpeed * dt;
		if (bg._offset.x < -_screenSize.x)
			bg._offset.x = (_BGTextures.size() - 1) * _screenSize.x - 1;
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer)
{
	for (auto& bg : _BGTextures)
	{
		SDL_Rect r;
		r.w = static_cast<int>(_screenSize.x);
		r.h = static_cast<int>(_screenSize.y);
		r.x = static_cast<int>(_owner->get_position().x - r.w / 2 + bg._offset.x);
		r.y = static_cast<int>(_owner->get_position().y - r.h / 2 + bg._offset.y);

		SDL_RenderCopy(renderer, bg._texture, nullptr, &r);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto t : textures)
	{
		BGTexture tmp;
		tmp._texture = t;
		tmp._offset.x = count * _screenSize.x;
		tmp._offset.y = 0;
		_BGTextures.emplace_back(tmp);
		count++;
	}
}
