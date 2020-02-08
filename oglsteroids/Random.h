#pragma  once
#include <random>
#include "Math.h"

class Random
{
public:
	static void Init();
	static void Seed(unsigned int seed);

	// Range: [0.0, 1.0]
	static float GetFloat();
	static float GetFloat(float min, float max);
	static int GetInt(int min, int max);

	static Vector2 GetVector(const Vector2& min, const Vector2& max);
private:
	static std::mt19937 _generator;
};
