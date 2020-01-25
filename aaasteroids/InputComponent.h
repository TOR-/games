#pragma once

#include <stdint.h>
#include <cstdio>

#include "MoveComponent.h"

class InputComponent : public MoveComponent
{
	public:
		// Update this early
		InputComponent(class Actor* owner):MoveComponent(owner){};
		InputComponent(class Actor* owner, int KFore, int KBack, int KTPos, int KTNeg,
				float magForeV, float magAngV)
			:MoveComponent(owner)
			,_magnitudeForwardVelocity(magForeV), _magnitudeAngularVelocity(magAngV)
		   	,_keyForward(KFore), _keyReverse(KBack), _keyPosTheta(KTPos), _keyNegTheta(KTNeg)
			{};

		void ProcessInput(const uint8_t* keyState) override;

		float get_magnitudeForwardVelocity () const { return _magnitudeForwardVelocity; }
		float get_magnitudeAngularVelocity () const { return _magnitudeAngularVelocity; }
		int   get_keyForward      () const { return _keyForward     ; }
		int   get_keyReverse      () const { return _keyReverse     ; }
		int   get_keyPosTheta     () const { return _keyPosTheta    ; }
		int   get_keyNegTheta     () const { return _keyNegTheta    ; }
		void set_magnitudeForwardVelocity (float f) { _magnitudeForwardVelocity = f; }
		void set_magnitudeAngularVelocity (float f) { _magnitudeAngularVelocity = f; }
		void set_keyForward      (int f)   { _keyForward      = f; }
		void set_keyReverse      (int f)   { _keyReverse      = f; }
		void set_keyPosTheta     (int f)   { _keyPosTheta     = f; }
		void set_keyNegTheta     (int f)   { _keyNegTheta     = f; }
	private:
		float _magnitudeForwardVelocity;
		float _magnitudeAngularVelocity;
		int _keyForward;
		int _keyReverse;
		int _keyPosTheta;
		int _keyNegTheta;
};
