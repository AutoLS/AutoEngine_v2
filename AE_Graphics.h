#ifndef AE_GRAPHICS
#define AE_GRAPHICS

enum AE_WindowFlags
{
	AE_WINDOW_RESIZABLE = 1,
	AE_WINDOW_HIDDEN = 2,
};

struct AE_DisplayInfo
{
	int RefreshRate;
	real32 GameUpdateHZ;
	real32 MsPerFrame;
	real32 TargetSecPerFrame;
	real32 FPS;
};

struct AE_Graphics
{
	SDL_Window* Window;
	SDL_Renderer* Renderer;
	
	AE_DisplayInfo DisplayInfo;
	
	rect32 WinRect;
};

AE_Graphics AE_GlobalGraphics = {};

AE_API int AE_GetWindowRefreshRate(SDL_Window *Window);
AE_API AE_DisplayInfo AE_GetDisplayInfo(SDL_Window* Window);

AE_API bool32 
AE_InitGraphics(AE_Graphics* Graphics, char* Title, v2 Dim, uint8 Flags);

void AE_LockFPS(AE_Clock* Clock, real32 TargetFPS);
real32 AE_GetFPS(AE_Clock* Clock);
#endif