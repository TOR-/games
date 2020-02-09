#include <GL/glew.h>

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

void SpriteComponent::Draw(Shader* shader)
{
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	_texture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &_texWidth, &_texHeight);
}
