#include "AE_Render.h"

AE_Surface AE_CreateSurface(int Width, int Height, int Depth)
{
	AE_Surface Ret = {};
	uint32 Rmask, Gmask, Bmask, Amask;
	
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Rmask = 0xff000000;
    Gmask = 0x00ff0000;
    Bmask = 0x0000ff00;
    Amask = 0x000000ff;
#else
    Rmask = 0x000000ff;
    Gmask = 0x0000ff00;
    Bmask = 0x00ff0000;
    Amask = 0xff000000;
#endif

	Ret.Data = SDL_CreateRGBSurface(0, Width, Height, Depth, 
								    Rmask, Gmask, Bmask, Amask);
									   
	return Ret;
};

AE_Surface AE_CreateSurfaceFromImage(char* Path)
{
	AE_Surface Ret = {};
	Ret.Data = IMG_Load(Path);
	
	return Ret;
}

void AE_FillSurface(AE_Surface* Surface, rect32* Rect, v4 Color)
{
	SDL_Rect Src;
	
	if(Rect) Src = ToSDLRect(Rect);
	SDL_Rect* pSrc = Rect ? &Src : 0;
	
	uint8 r = RoundReal32ToUint8(Color.r * 255.0f);
	uint8 g = RoundReal32ToUint8(Color.g * 255.0f);
	uint8 b = RoundReal32ToUint8(Color.b * 255.0f);
	SDL_FillRect(Surface->Data, pSrc, SDL_MapRGB(Surface->Data->format, r, g, b));
}

AE_Texture 
AE_LoadTextureFromSurface(AE_Graphics* Graphics, AE_Surface* Surface, real32 a)
{
	if(!Surface->Data)
	{
		printf("Loading image failed! IMG_Error: %s\n", IMG_GetError());
		return {};
	}
	else
	{
		AE_Texture Ret = {};
		Ret.Rect = {0, 0, (real32)Surface->Data->w, (real32)Surface->Data->h};
		Ret.Data = SDL_CreateTextureFromSurface(Graphics->Renderer, Surface->Data);
		Ret.Alpha = a;
		if(!Ret.Data)
		{
			printf("Creating texture failed! SDL_Error: %s\n", SDL_GetError());
			return {};
		}
		else
		{
			SDL_SetTextureBlendMode(Ret.Data, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(Ret.Data, RoundReal32ToUint8(a * 255));
			return Ret;
		}
	}
	
	return {};
}

AE_Texture AE_LoadTexture(AE_Graphics* Graphics, char* Path, real32 a)
{
	SDL_Surface* Surface = IMG_Load(Path);
	if(!Surface)
	{
		printf("Loading image failed! IMG_Error: %s\n", IMG_GetError());
		SDL_FreeSurface(Surface);
		return {};
	}
	else
	{
		AE_Texture Ret = {};
		Ret.Rect = {0, 0, (real32)Surface->w, (real32)Surface->h};
		Ret.Data = SDL_CreateTextureFromSurface(Graphics->Renderer, Surface);
		Ret.Alpha = a;
		if(!Ret.Data)
		{
			printf("Creating texture failed! SDL_Error: %s\n", SDL_GetError());
			return {};
		}
		else
		{
			SDL_SetTextureBlendMode(Ret.Data, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(Ret.Data, RoundReal32ToUint8(a * 255));
			SDL_FreeSurface(Surface);
			return Ret;
		}
	}
	
	return {};
}

void AE_DrawTexture(AE_Graphics* Graphics, AE_Texture* Texture, 
				    rect32* Src, rect32* Dst)
{
	SDL_Rect SrcRect = {}; 
	SDL_Rect DstRect = {};
	
	if(Src) SrcRect = ToSDLRect(Src);
	if(Dst) DstRect = ToSDLRect(Dst);
	
	SDL_Rect* pSrc = Src ? &SrcRect : 0;
	SDL_Rect* pDst = Dst ? &DstRect : 0;
	
	SDL_RenderCopy(Graphics->Renderer, Texture->Data, pSrc, pDst);
}

AE_RenderText AE_LoadRenderText(AE_Graphics* Graphics, v4 TextColor,
								TTF_Font* Font, const char* Text)
{
	AE_RenderText Ret = {};
	if(Text)
	{
		SDL_Color TextureColor = ToSDLColor(TextColor);
		SDL_Surface* Surface = TTF_RenderText_Solid(Font, Text, TextureColor);
		if(!Surface)
		{
			printf("Loading text failed! TTF_Error: %s\n", TTF_GetError());
		}
		else
		{
			Ret.Font = Font;
			Ret.Dim.x = Surface->w;
			Ret.Dim.y = Surface->h;
			Ret.Texture = 
			SDL_CreateTextureFromSurface(Graphics->Renderer, Surface);
			if(!Ret.Texture)
			{
				printf("Creating texture failed! SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				SDL_SetTextureBlendMode(Ret.Texture, SDL_BLENDMODE_BLEND);
				SDL_SetTextureAlphaMod(Ret.Texture, 255);
			}
		}
		SDL_FreeSurface(Surface);
	}
	
	return Ret;
}

void AE_DrawRenderText(AE_Graphics* Graphics, AE_RenderText* RenderText, 
					   rect32* Src, rect32* Dst)
{
	SDL_Rect SrcRect = {}; 
	SDL_Rect DstRect = {};
	
	if(Src) SrcRect = ToSDLRect(Src);
	if(Dst) DstRect = ToSDLRect(Dst);
	
	SDL_Rect* pSrc = Src ? &SrcRect : 0;
	SDL_Rect* pDst = Dst ? &DstRect : 0;
	
	SDL_RenderCopy(Graphics->Renderer, RenderText->Texture, pSrc, pDst);
}

void AE_RenderClear(AE_Graphics* Graphics, v4 ClearColor)
{
	uint8 r = RoundReal32ToUint8(ClearColor.r * 255.0f);
	uint8 g = RoundReal32ToUint8(ClearColor.g * 255.0f);
	uint8 b = RoundReal32ToUint8(ClearColor.b * 255.0f);
	uint8 a = RoundReal32ToUint8(ClearColor.a * 255.0f);
	SDL_SetRenderDrawColor(Graphics->Renderer, r, g, b, a);
	SDL_RenderClear(Graphics->Renderer);
}

void AE_RenderShow(AE_Graphics* Graphics)
{
	SDL_RenderPresent(Graphics->Renderer);
}