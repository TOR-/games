#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int draw_order)
	: SpriteComponent(owner, draw_order), _currentFrame(0.0f), _animFPS(30.0f) {}

void AnimSpriteComponent::Update(float dt)
{
	SpriteComponent::Update(dt);

	if (_animTextures.size() > 0)
	{
		// Update current frame, wrapping as needed
		_currentFrame += _animFPS * dt;
		while (_currentFrame >= _animTextures.size())
			_currentFrame -= _animTextures.size();

		SetTexture(_animTextures[static_cast<int>(_currentFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	_animTextures = textures;
	if (_animTextures.size() > 0)
	{
		_currentFrame = 0.0f;
		SetTexture(_animTextures[0]);
	}
}

