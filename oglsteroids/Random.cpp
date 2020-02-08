#include "Random.h"

void Random::Init()
{
	std::random_device rd;
	Random::Seed(rd());
}

void Random::Seed(unsigned int seed)
{
	_generator.seed(seed);
}

float Random::GetFloat()
{
	return GetFloat(0.0f, 1.0f);
}

float Random::GetFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(_generator);
}

int Random::GetInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(_generator);
}

Vector2 Random::GetVector(const Vector2& min, const Vector2& max)
{
	Vector2 r = Vector2(GetFloat(), GetFloat());
	return min + (max - min) * r;
}

std::mt19937 Random::_generator;
