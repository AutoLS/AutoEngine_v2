#include "AE_UI.h"

bool AE_IsHot(int Id)
{
	return Id == AE_GlobalUIState.Hot;
}

bool AE_IsActive(int Id)
{
	return Id == AE_GlobalUIState.Active;
}

void AE_UIStart()
{
	AE_GlobalUIState.Hot = 0;
	AE_GlobalUIState.IDCounter = 1;
}

int AE_GenUID()
{
	return AE_GlobalUIState.IDCounter++;
}

void AE_UIFinish()
{
	if(!AE_GlobalUIState.MouseDown)
	{
		AE_GlobalUIState.Active = 0;
	}
	else
	{
		if(AE_GlobalUIState.Active == 0)
		{
			AE_GlobalUIState.Active = -1;
		}
	}
}

bool AE_Button(int Id, AE_Core* Core, rect32 Rect, 
			   AE_Texture* Click, AE_Texture* Idle, 
			   TTF_Font* Font, const char* Text, v4 TextColor)
{
	AE_Input* Input = &Core->Input;
	AE_Graphics* Graphics = Core->Graphics;
	
	if(IsPointInRect(V2(Input->MouseState), &Rect))
	{
		AE_GlobalUIState.Hot = Id;
		if(AE_GlobalUIState.MouseDown)
		{
			AE_GlobalUIState.Active = Id;
		}
	}
	
	if(AE_IsHot(Id))
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
	
	if(Text)
	{
		AE_RenderText ButtonText = 
		AE_LoadRenderText(Graphics, TextColor, Font, Text);
		rect32 ButtonRect = {V2(), V2(ButtonText.Dim)};
		SetRect32ScreenSpace(&ButtonRect, &Rect, V2(), POSITION_CENTERED);
		
		AE_DrawRenderText(Graphics, &ButtonText, 0, &ButtonRect);
		SDL_DestroyTexture(ButtonText.Texture);	
	}

	return !AE_GlobalUIState.MouseDown && AE_IsActive(Id) && AE_IsHot(Id);
}

void AE_Scrollbar(int Id, AE_Core* Core, AE_ScrollbarData* Scrollbar,
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
		if(IsPointInRect(V2(Input->MouseState), &GripRect))
		{
			if(!Scrollbar->Active)
			{
				Scrollbar->Active = true;
				Scrollbar->LastMouseY = (real32)Input->MouseState.y;
				Scrollbar->LastGripY = Scrollbar->GripY;
			}
			
			AE_GlobalUIState.Hot = Id;
			AE_GlobalUIState.Active = Id;
		}
		
		if(Scrollbar->Active)
		{
			Scrollbar->VelY = 0;
			real32 DeltaY = Input->MouseState.y - Scrollbar->LastMouseY;
			Scrollbar->GripY = Scrollbar->LastGripY + DeltaY;
			
			Scrollbar->CamY = TrackHeight * (Scrollbar->GripY / ViewRect.Dim.y);
			
			if(Scrollbar->GripY < ViewRect.Pos.y)
			{
				Scrollbar->GripY = ViewRect.Pos.y;
				Scrollbar->CamY = 0;
			}
			
			if(Scrollbar->GripY + GripHeight > ViewRect.Pos.y + ViewRect.Dim.y)
			{
				Scrollbar->GripY = ViewRect.Pos.y + 
									   ViewRect.Dim.y - GripHeight;
				Scrollbar->CamY = TrackHeight * (Scrollbar->GripY/ViewRect.Dim.y);
			}
			
			GripRect.Pos.y = Scrollbar->GripY;
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
	if(Scrollbar->GripY + GripHeight > ViewRect.Pos.y + ViewRect.Dim.y)
	{
		Scrollbar->VelY = 0;
		Scrollbar->GripY = ViewRect.Pos.y + ViewRect.Dim.y - GripHeight;
		Scrollbar->CamY = TrackHeight * (Scrollbar->GripY / ViewRect.Dim.y);
	}
		
	SDL_Rect RenderGripRect = ToSDLRect(&GripRect);
	SDL_SetRenderDrawColor(Graphics->Renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(Graphics->Renderer, &RenderGripRect);
}
