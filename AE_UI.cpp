#include "AE_UI.h"

bool AE_Button(AE_Core* Core, rect32 Rect, 
			   AE_Texture* Click, AE_Texture* Idle, 
			   TTF_Font* Font, const char* Text, v4 TextColor)
{
	AE_Input* Input = &Core->Input;
	AE_Graphics* Graphics = Core->Graphics;
	
	bool Hot = false;
	bool Active = false;
	if(Input->Mouse.Keys[AE_MB_LEFT].EndedDown)
	{
		if(IsPointInRect(V2(Input->MouseState), &Rect))
		{
			Active = true;
		}
	}
	else if(IsPointInRect(V2(Input->MouseState), &Rect))
	{
		Hot = true;
	}
	
	if(Active || Hot)
	{
		if(Click) AE_DrawTexture(Graphics, Click, 0, &Rect);
		else
		{
			SDL_Rect FillRect = ToSDLRect(&Rect);
			SDL_SetRenderDrawColor(Graphics->Renderer, 15, 15, 15, 100);
			SDL_RenderFillRect(Graphics->Renderer, &FillRect);
		}			
	}
	else
	{
		if(Idle) AE_DrawTexture(Graphics, Idle, 0, &Rect);
		else 
		{
			SDL_Rect FillRect = ToSDLRect(&Rect);
			SDL_SetRenderDrawColor(Graphics->Renderer, 150, 150, 150, 255);
			SDL_RenderFillRect(Graphics->Renderer, &FillRect);
		}
	}
	
	AE_RenderText ButtonText = AE_LoadRenderText(Graphics, TextColor, Font, Text);
	rect32 ButtonRect = {V2(), V2(ButtonText.Dim)};
	SetRect32ScreenSpace(&ButtonRect, &Rect, V2(), POSITION_CENTERED);
	
	AE_DrawRenderText(Graphics, &ButtonText, 0, &ButtonRect);
	SDL_DestroyTexture(ButtonText.Texture);
	
	return Active;
}

void AE_Scrollbar(AE_Core* Core, AE_ScrollbarData* Scrollbar,
				  rect32 ViewRect, real32 TrackHeight, real32 GripWidth,
				  real32 t)
{
	AE_Input* Input = &Core->Input;
	AE_Graphics* Graphics = Core->Graphics;
	
	real32 GripHeight = (ViewRect.Dim.y / TrackHeight) * ViewRect.Dim.y;
	rect32 GripRect = {V2(), V2(GripWidth, GripHeight)};
	SetRect32ScreenSpace(&GripRect, &ViewRect, V2(), POSITION_TOP_RIGHT);
	GripRect.Pos.y = Scrollbar->GripY;
	
	if(Input->Mouse.Keys[AE_MB_LEFT].IsDown)
	{
		Scrollbar->AccelY = 0;
		if((IsPointInRect(V2(Input->MouseState), &GripRect) || Scrollbar->Active))
		{
			Scrollbar->VelY = 0;
			real32 DeltaY = Input->MouseState.y - Scrollbar->GripY;
			Scrollbar->GripY += DeltaY - GripHeight*0.5f;
			
			Scrollbar->CamY = TrackHeight * (Scrollbar->GripY / ViewRect.Dim.y);
						
			if(Scrollbar->GripY < ViewRect.Pos.y)
			{
				Scrollbar->GripY = ViewRect.Pos.y;
				DeltaY = 0;
				Scrollbar->CamY = 0;
			}
			if(Scrollbar->GripY + GripHeight > 
			   ViewRect.Pos.y + ViewRect.Dim.y)
			{
				DeltaY = 0;
				Scrollbar->GripY = ViewRect.Pos.y + 
									   ViewRect.Dim.y - GripHeight;
				Scrollbar->CamY = TrackHeight * (Scrollbar->GripY/ViewRect.Dim.y);
			}
			GripRect.Pos.y = Scrollbar->GripY;
			Scrollbar->Active = true;
		}
	}
	else if(Input->Scrolling)
	{
		if(IsPointInRect(V2(Input->MouseState), &ViewRect))
		{
			Input->Scrolling = false;
			
			if(Input->Scroll.y < 0 && Scrollbar->VelY > 0)
			{
				Scrollbar->VelY = 0;
			}
			if(Input->Scroll.y > 0 && Scrollbar->VelY < 0)
			{
				Scrollbar->VelY = 0;
			}
			
			//NOTE: Negating Y here because Y axis is reversed on screen space
			Scrollbar->AccelY = -Input->Scroll.y * 30000.0f;
			
			Scrollbar->Active = true;
		}
	}
	else
	{
		Scrollbar->AccelY = 0;
	}
	
	if(!Input->Mouse.Keys[AE_MB_LEFT].IsDown)
	{
		Scrollbar->Active = false;
	}
	
	Scrollbar->AccelY -= Scrollbar->VelY * 4.0f;
	Scrollbar->VelY += Scrollbar->AccelY * t;
	Scrollbar->GripY += Scrollbar->VelY * t;
	
	Scrollbar->CamY = TrackHeight * (Scrollbar->GripY / ViewRect.Dim.y);
	if(Scrollbar->GripY < ViewRect.Pos.y)
	{
		Scrollbar->VelY = 0;
		Scrollbar->GripY = ViewRect.Pos.y;
		Scrollbar->CamY = 0;
	}
	if(Scrollbar->GripY + GripHeight > 
	   ViewRect.Pos.y + ViewRect.Dim.y)
	{
		Scrollbar->VelY = 0;
		Scrollbar->GripY = ViewRect.Pos.y + ViewRect.Dim.y - GripHeight;
		Scrollbar->CamY = TrackHeight * (Scrollbar->GripY / ViewRect.Dim.y);
	}
	
	SDL_Rect RenderGripRect = ToSDLRect(&GripRect);
	SDL_SetRenderDrawColor(Graphics->Renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(Graphics->Renderer, &RenderGripRect);
}
