#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(Actor* owner, int draw_order)
	: Component(owner), _texture(nullptr), _drawOrder(draw_order), _texWidth(0), _texHeight(0)
{
	_owner->get_game()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	_owner->get_game()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if(_texture)
	{
		// Scale and center sprite by owner's scale
		SDL_Rect r;
	r.w = static_cast<int>(_texWidth  * _owner->get_scale());
	r.h = static_cast<int>(_texHeight * _owner->get_scale());
	r.x = static_cast<int>(_owner->get_position().x - r.w / 2);
	r.y = static_cast<int>(_owner->get_position().y - r.h / 2);

	SDL_RenderCopyEx( renderer, _texture, nullptr, &r,
			-Math::rad_to_deg(_owner->get_rotation()),
			/* SDL positive angle is clockwise, oppposite to unit circle */
			nullptr, SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	_texture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &_texWidth, &_texHeight);
}
