#ifndef AE_TIME
#define AE_TIME

struct AE_Clock
{
	uint64 PerformanceFrequency;
	real32 SecPerCount;
	
	uint64 LastCount;
	uint64 EndCount;
	
	uint64 CounterElapsed;
	
	real32 t;
};

inline real32 
AE_GetSecondsElapsed(AE_Clock* Clock);
inline real32 
AE_GetSecondsElapsed(uint64 LastCount, uint64 CurrentCount, real32 SecPerCount);

AE_API AE_Clock AE_InitClock();
AE_API inline void AE_UpdateClock(AE_Clock* Clock);

struct AE_Timer
{
	real32 t;
	real32 tEnd;
	real32 DefEndTime;
	
	bool Active;
	bool Complete;
};

AE_API inline AE_Timer 
AE_CreateTimer(real32 tEnd, bool Active);
AE_API inline void AE_StartTimer(AE_Timer* Timer);
AE_API inline void AE_StopTimer(AE_Timer* Timer);
AE_API inline void AE_ResetTimer(AE_Timer* Timer);
AE_API void AE_ResetAndStopTimer(AE_Timer* Timer);
AE_API void AE_ProcessTimer(AE_Timer* Timer, real32 t);

#endif