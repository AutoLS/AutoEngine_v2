#ifndef AE_UI
#define AE_UI

#ifdef AE_IMGUI_SRC_ID
#define AE_GEN_ID ((AE_IMGUI_SRC_ID) + (__LINE__))
#else
#define AE_GEN_ID (__LINE__)
#endif

struct AE_UIState
{
	v2i MouseState;
	int Hot;
	int Active;
	bool32 MouseDown;
	
	int IDCounter;
};

AE_GLOBAL_VARIABLE AE_UIState AE_GlobalUIState;

AE_API bool AE_IsHot(int Id);
AE_API bool AE_IsActive(int Id);
AE_API int AE_GenID();

AE_API void AE_UIStart();
AE_API void AE_UIFinish();

AE_API bool 
AE_Button(int Id, AE_Core* Core, rect32 Rect, 
		  AE_Texture* Click, AE_Texture* Idle, 
		  TTF_Font* Font, const char* Text, v4 TextColor);

struct AE_ScrollbarData
{
	real32 AccelY;
	real32 VelY;
	real32 DeltaY;
	real32 CamY;
	real32 GripY;
	real32 LastGripY;
	real32 LastMouseY;
	bool Active;
};

AE_API void 
AE_Scrollbar(int Id, AE_Core* Core, AE_ScrollbarData* Scrollbar,
			 rect32 ViewRect, real32 TrackHeight, real32 GripWidth,
			 real32 t);
#endif