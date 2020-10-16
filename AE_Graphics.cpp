#include "AE_Graphics.h"

int AE_GetWindowRefreshRate(SDL_Window *Window)
{
    SDL_DisplayMode Mode;
    int DisplayIndex = SDL_GetWindowDisplayIndex(Window);
    // If we can't find the refresh rate, we'll return this:
    int DefaultRefreshRate = 60;
    if (SDL_GetDesktopDisplayMode(DisplayIndex, &Mode) != 0)
    {
        return DefaultRefreshRate;
    }
    if (Mode.refresh_rate == 0)
    {
        return DefaultRefreshRate;
    }
    return Mode.refresh_rate;
}

AE_DisplayInfo AE_GetDisplayInfo(SDL_Window* Window)
{
	AE_DisplayInfo Display = {};
	Display.RefreshRate = AE_GetWindowRefreshRate(Window);
	Display.GameUpdateHZ = (real32)Display.RefreshRate;
	Display.MsPerFrame = 1000.0f / Display.GameUpdateHZ;
	Display.TargetSecPerFrame = 1.0f / Display.GameUpdateHZ;
	
	return Display;
}

bool32 AE_InitGraphics(AE_Graphics* Graphics, char* Title, v2 Dim, 
							  uint8 Flags)
{
	AE_SetLastError(AE_ERR_NONE);
	
	uint32 SDLWindowFlags = 0;
	
	SDLWindowFlags |= SDL_WINDOW_OPENGL;

	if(Flags & AE_WINDOW_RESIZABLE) SDLWindowFlags |= SDL_WINDOW_RESIZABLE;
	if(Flags & AE_WINDOW_HIDDEN) 	SDLWindowFlags |= SDL_WINDOW_HIDDEN;
	
	Graphics->WinRect = {V2(), Dim};
	Graphics->Window = 
	SDL_CreateWindow(Title, 
					 SDL_WINDOWPOS_UNDEFINED, 
					 SDL_WINDOWPOS_UNDEFINED, 
					 (int)Dim.x, (int)Dim.y,
					 SDLWindowFlags);
					 
	if(!Graphics->Window)
	{
		AE_SetLastError(AE_ERR_CREATE_WINDOW_FAILURE);
		return AE_GetLastError();
	}
	else
	{
		if(AE_IsOpenGL)
		{
			SDL_GLContext GLContext = SDL_GL_CreateContext(Graphics->Window);
			if(!GLContext)
			{
				//printf("Failed to create context: %s", SDL_GetError());
				AE_SetLastError(AE_ERR_GL_CREATE_CONTEXT_FAILURE);
				return AE_GetLastError();
			}
			else
			{
				if(SDL_GL_MakeCurrent(Graphics->Window, GLContext) != 0)
				{
					//printf("Failed to make current context: %s", SDL_GetError());
					AE_SetLastError(AE_ERR_GL_MAKE_CURRENT_CONTEXT_FAILURE);
					return AE_GetLastError();
				}
				else
				{
					GLenum err = glewInit();
					if (GLEW_OK != err)
					{
						AE_SetLastError(AE_LIB_ERR_GLEW_LOAD_FAILURE);
						return AE_GetLastError();
					}
				}
			}
		}
		
		Graphics->Renderer = 
		SDL_CreateRenderer(Graphics->Window, -1, SDL_RENDERER_ACCELERATED);
		if(!Graphics->Renderer)
		{
			//printf("Create Renderer failed: %s\n", SDL_GetError());
			AE_SetLastError(AE_ERR_CREATE_RENDERER_FAILURE);
			return AE_GetLastError();
		}
		
		Graphics->DisplayInfo = AE_GetDisplayInfo(Graphics->Window);
	}
	
	return AE_GetLastError();
}

void AE_LockFPS(AE_Clock* Clock, real32 TargetFPS)
{	
	real32 FrameCompleteTime = 
	AE_GetSecondsElapsed(Clock->LastCount, SDL_GetPerformanceCounter(),	
						 Clock->SecPerCount);
	real32 TargetSecPerFrame = 1.0f / TargetFPS;
	
	if(FrameCompleteTime < TargetSecPerFrame)
	{
		int SleepTime = (int)((TargetSecPerFrame - FrameCompleteTime) * 1000) - 1;
		if(SleepTime > 0)
		{
			SDL_Delay(SleepTime);
		}
		//assert(Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter()) < TargetSecPerFrame);
		while(AE_GetSecondsElapsed(Clock->LastCount, SDL_GetPerformanceCounter(),
								   Clock->SecPerCount) < TargetSecPerFrame);
	}
	
	Clock->EndCount = SDL_GetPerformanceCounter();
}

real32 AE_GetFPS(AE_Clock* Clock)
{
	if(Clock->CounterElapsed == 0) return -1;
	real32 Ret = (real32)Clock->PerformanceFrequency / 
				 (real32)Clock->CounterElapsed;
	
	return Ret;
}
