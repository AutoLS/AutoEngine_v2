#ifndef AE_UI
#define AE_UI

AE_API bool 
AE_Button(AE_Core* Core, rect32 Rect, 
		  AE_Texture* Click, AE_Texture* Idle, 
		  TTF_Font* Font, const char* Text, v4 TextColor);

struct AE_ScrollbarData
{
	real32 AccelY;
	real32 VelY;
	real32 DeltaY;
	real32 CamY;
	real32 GripY;
	bool Active;
};

AE_API void 
AE_Scrollbar(AE_Core* Core, AE_ScrollbarData* Scrollbar,
			 rect32 ViewRect, real32 TrackHeight, real32 GripWidth,
			 real32 t);
#endif