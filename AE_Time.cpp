#include "AE_Time.h"

AE_Clock AE_InitClock()
{
	AE_Clock Ret = {};
	Ret.PerformanceFrequency = SDL_GetPerformanceFrequency();
	Ret.SecPerCount = 1.0f / (real32)Ret.PerformanceFrequency;
	Ret.LastCount = SDL_GetPerformanceCounter();
	
	return Ret;
};

inline real32 AE_GetSecondsElapsedFromClock(AE_Clock* Clock)
{
	real32 Result = (real32)((Clock->EndCount - Clock->LastCount) * 
							  Clock->SecPerCount);
	return Result;
}

inline real32 AE_GetSecondsElapsed(uint64 LastCount, uint64 CurrentCount, 
								   real32 SecPerCount)
{
	real32 Result = (real32)((CurrentCount - LastCount) * SecPerCount);
	return Result;
}

inline void AE_UpdateClock(AE_Clock* Clock)
{
	Clock->t = AE_GetSecondsElapsedFromClock(Clock);
	Clock->CounterElapsed = Clock->EndCount - Clock->LastCount;
	Clock->LastCount = Clock->EndCount;
}

inline AE_Timer 
AE_CreateTimer(real32 tEnd, bool Active)
{
	AE_Timer Ret = {};
	Ret.tEnd = tEnd;
	Ret.DefEndTime = tEnd;
	Ret.Active = Active;
	
	return Ret;
};

inline void StartTimer(AE_Timer* Timer)
{
	Timer->Complete = false;
	Timer->Active = true;
}

inline void StopTimer(AE_Timer* Timer)
{
	Timer->Active = false;
}

inline void ResetTimer(AE_Timer* Timer)
{
	Timer->t = 0;
}

void ResetAndStopTimer(AE_Timer* Timer)
{
	ResetTimer(Timer);
	StopTimer(Timer);
}

void ProcessTimer(AE_Timer* Timer, real32 t)
{
	if(Timer->Active)
	{
		Timer->t += t;
		if(Timer->t >= Timer->tEnd && Timer->tEnd > 0)
		{
			Timer->Complete = true;
			ResetAndStopTimer(Timer);
		}
		else if(Timer->tEnd == 0)
		{
			Timer->Complete = true;
		}
	}
}