#include "AutoEngine.h"

int wmain()
{	
	if(AE_InitLib(AE_LIB_SDL_FULL) == 0)
	{
		printf("Init lib successfully!\n");
		AE_Graphics Graphics = {};
		
		if(AE_InitGraphics(&Graphics, "Test", V2(1280, 720), 0) == 0)
		{
			printf("Init graphics successfully!\n");
			
			AE_Core Core = AE_InitCore(&Graphics);
			
			bool Running = true;
			
			while(Running)
			{
				if(AE_HandleEvents(&Core))
				{
					Running = false;
				}
				
				AE_RenderClear(&Graphics, Color(0, 100, 150));
				//Update
				
				AE_LockFPS(&Core.Clock, 144);
				
				//Render
				AE_RenderShow(&Graphics);
				
				AE_UpdateClock(&Core.Clock);
				printf("FPS: %.1f\n", AE_GetFPS(&Core.Clock));
			}
		}
		else
		{
			AE_PrintLastError();
		}
	}
	else
	{
		AE_PrintLastError();
	}
	
	return 0;
}
