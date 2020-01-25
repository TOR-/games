#include "CircleComponent.h"

bool Intersect(const CircleComponent& ca, const CircleComponent& cb)
{
	float dO_sq = (ca.get_center() - cb.get_center()).LengthSq();
	float sum_r = ca.get_radius() + cb.get_radius();
	return dO_sq <= sum_r*sum_r;
}
