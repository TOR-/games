#include "InputComponent.h"

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	float foreSpeed = 0.0f;
	if (keyState[_keyForward])
		foreSpeed += _magnitudeForwardVelocity;
	if (keyState[_keyReverse])
		foreSpeed -= _magnitudeForwardVelocity;
	set_forwardSpeed(foreSpeed);

	float dTheta = 0.0f;
	if (keyState[_keyPosTheta])
		dTheta += _magnitudeAngularVelocity;
	if (keyState[_keyNegTheta])
		dTheta -= _magnitudeAngularVelocity;
	set_angularSpeed(dTheta);
}
