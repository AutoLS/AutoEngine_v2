#ifndef AE_RENDER
#define AE_RENDER

struct AE_Surface
{
	SDL_Surface* Data;
};

AE_API AE_Surface AE_CreateSurface(int Width, int Height, int Depth = 32);
AE_API AE_Surface AE_CreateSurfaceFromImage(char* Path);
AE_API void AE_FillSurface(AE_Surface* Surface, rect32* Rect, v4 Color);

struct AE_Texture
{
	rect32 Rect;
	SDL_Texture* Data;
	
	real32 Alpha;
};

AE_API AE_Texture 
AE_LoadTextureFromSurface(AE_Graphics* Graphics, AE_Surface* Surface, real32 a = 1);
AE_API AE_Texture 
AE_LoadTexture(AE_Graphics* Graphics, char* Path, real32 a = 1);
AE_API void AE_DrawTexture(AE_Graphics* Graphics, AE_Texture* Texture, 
						   rect32* Src, rect32* Dst);
AE_API void AE_RenderClear(AE_Graphics* Graphics, v4 ClearColor);
AE_API void AE_RenderShow(AE_Graphics* Graphics);

#endif