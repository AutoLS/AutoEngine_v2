#include "AE_Input.h"

void AE_ProcessKeyPress(AE_ButtonState* NewState, bool IsDown, bool WasDown)
{
	NewState->IsDown = IsDown;
	NewState->EndedDown = IsDown;
	NewState->Up = WasDown;
	++NewState->HalfTransitions;
}

void AE_ResetKeyState(AE_ControllerInput* KeyInput, int nKeys)
{
	for(int i = 0; i < nKeys; ++i)
	{
		KeyInput->Keys[i].EndedDown = 0;
	}
}