#include "AutoEngine.h"

bool32 AE_GlobalLastError;
bool32 AE_IsOpenGL;

void AE_PrintLastError()
{
	switch(AE_GlobalLastError)
	{
		case AE_ERR_NONE:
		{
			printf("There are no errors.\n");
		} break;
		case AE_LIB_ERR_FLAG_IS_ZERO:
		{
			printf("[ERROR: AE_LIB_ERR_FLAG_IS_ZERO] Setting flags to 0 is not allowed.\n");
		} break;
		case AE_LIB_ERR_SDL_INIT_FAILURE:
		{
			printf("[ERROR: AE_LIB_ERR_SDL_INIT_FAILURE] Failed to init SDL. %s\n", SDL_GetError());
		} break; 
		case AE_LIB_ERR_SDL_IMG_FAILURE:
		{
			printf("[ERROR: AE_LIB_ERR_SDL_IMG_FAILURE] Failed to init SDL_IMG. %s\n", 
					IMG_GetError());
		} break;
		case AE_LIB_ERR_SDL_MIX_FAILURE:
		{
			printf("[ERROR: AE_LIB_ERR_SDL_MIX_FAILURE] Failed to init SDL_Mixer. %s\n", 
					Mix_GetError());
		} break;
		case AE_LIB_ERR_SDL_TTF_FAILURE:
		{
			printf("[ERROR: AE_LIB_ERR_SDL_TTF_FAILURE] Failed to init SDL_TTF. %s\n", 
					TTF_GetError());
		} break;
		case AE_LIB_ERR_SDL_NET_FAILURE:
		{
			printf("[ERROR: AE_LIB_ERR_SDL_NET_FAILURE] Failed to init SDL_NET. %s\n", 
					SDLNet_GetError());
		} break;
		case AE_ERR_CREATE_WINDOW_FAILURE:
		{
			printf("[ERROR: AE_ERR_CREATE_WINDOW_FAILURE] Failed to create window. %s\n", SDL_GetError());
		} break;
		case AE_ERR_CREATE_RENDERER_FAILURE:
		{
			printf("[ERROR: AE_ERR_CREATE_RENDERER_FAILURE] Failed to create renderer. %s\n", SDL_GetError());
		} break;
		case AE_ERR_GL_CREATE_CONTEXT_FAILURE:
		{
			printf("[ERROR: AE_ERR_GL_CREATE_CONTEXT_FAILURE] Failed to create OpenGL context. %s\n", SDL_GetError());
		} break;
		case AE_ERR_GL_MAKE_CURRENT_CONTEXT_FAILURE:
		{
			printf("[ERROR: AE_ERR_GL_MAKE_CURRENT_CONTEXT_FAILURE] Failed to make current OpenGL context. %s\n", SDL_GetError());
		} break;
	}
}

bool32 AE_InitLib(uint8 Flags)
{
	AE_SetLastError(AE_ERR_NONE);
	
	if(Flags == 0)
	{
		//printf("Error: Cannot set flags to 0\n");
		AE_SetLastError(AE_LIB_ERR_FLAG_IS_ZERO);
		return AE_GetLastError();
	}
	if(Flags & AE_LIB_SDL)
	{
		if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) 
		{
			//printf("Unable to initialize SDL: %s\n", SDL_GetError());
			AE_SetLastError(AE_LIB_ERR_SDL_INIT_FAILURE);
			return AE_GetLastError();
		}
	}
	if(Flags & AE_LIB_SDL_FULL)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		{
			//printf("Unable to initialize SDL: %s\n", SDL_GetError());
			AE_SetLastError(AE_LIB_ERR_SDL_INIT_FAILURE);
			return AE_GetLastError();
		}
		else
		{
			int Flags = IMG_INIT_JPG|IMG_INIT_PNG;
			int Initted = IMG_Init(Flags);
			if((Initted&Flags) != Flags) 
			{
				//printf("IMG_Init: Failed to init required jpg and png support!\n");
				//printf("IMG_Init: %s\n", IMG_GetError());
				AE_SetLastError(AE_LIB_ERR_SDL_IMG_FAILURE);
				return AE_GetLastError();
			}
			int MusicFlags = MIX_INIT_OGG | MIX_INIT_MOD;
			int MixInitted = Mix_Init(MusicFlags);
			if(MixInitted && MusicFlags != MusicFlags) 
			{
				//printf("Mix_Init: Failed to init required ogg and mod support!\n");
				//printf("Mix_Init: %s\n", Mix_GetError());
				AE_SetLastError(AE_LIB_ERR_SDL_MIX_FAILURE);
				return AE_GetLastError();
			}
			else
			{
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) 
				{
					//printf("Mix_OpenAudio: %s\n", Mix_GetError());
					AE_SetLastError(AE_LIB_ERR_SDL_MIX_FAILURE);
					return AE_GetLastError();
				}
			}
			if(TTF_Init() == -1) 
			{
				//printf("TTF_Init: %s\n", TTF_GetError());
				AE_SetLastError(AE_LIB_ERR_SDL_TTF_FAILURE);
				return AE_GetLastError();
			}
				
			if(SDLNet_Init() == -1) 
			{
				//printf("SDLNet_Init: %s\n", SDLNet_GetError());
				AE_SetLastError(AE_LIB_ERR_SDL_NET_FAILURE);
				return AE_GetLastError();
			}
		}
	}
	
	if(Flags & AE_LIB_OPENGL)
	{
		AE_IsOpenGL = 1;
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	}
	
	return AE_GetLastError();
}

AE_Core AE_InitCore(AE_Graphics* Graphics)
{
	AE_Core Result = {};
	Result.Graphics = Graphics;
	Result.Input.KeyState = SDL_GetKeyboardState(0);
	Result.Clock = AE_InitClock();
	
	return Result;
}

bool AE_HandleEvents(AE_Core* Core)
{
	AE_Input* Input = &Core->Input;
	AE_Graphics* Graphics = Core->Graphics;
	
	SDL_Event* Event = &Core->Event;
	
	bool Quit = false;
	
	AE_ResetKeyState(&Input->Keyboard, AEK_MAX);
	AE_ResetKeyState(&Input->Mouse, AE_MB_MAX);
	
	while(SDL_PollEvent(Event) != 0)
	{
		switch(Event->type)
		{
			case SDL_QUIT:
			{
				Quit = true;
			} break;
			case SDL_WINDOWEVENT:
			{
				switch(Event->window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
					{
						v2i Dim;
						SDL_GetWindowSize(Graphics->Window, 
										  &Dim.x, 
										  &Dim.y);
						Graphics->WinRect = {V2(), V2(Dim)};
						
						if(AE_IsOpenGL) glViewport(0, 0, Dim.x, Dim.y);
					} break;
				}
			} break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				bool IsDown = (Event->key.state == SDL_PRESSED);
				bool WasDown = false;
				if(Event->key.state == SDL_RELEASED)
				{
					WasDown = true;
				}
				else if(Event->key.repeat != 0)
				{
					WasDown = true;
				}
				
				//bool Down = Event->type == SDL_KEYDOWN;
				Input->KeyDown = IsDown;
				Input->KeyWasDown = WasDown;
				Input->KeyEndedDown = (Event->key.repeat == 0) && IsDown;
				
				if(Event->key.repeat == 0)
				{
					switch(Event->key.keysym.sym)
					{
						case SDLK_0:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_0], IsDown, WasDown); break;   
						case SDLK_1:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_1], IsDown, WasDown); break; 
						case SDLK_2:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_2], IsDown, WasDown); break; 
						case SDLK_3:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_3], IsDown, WasDown); break; 
						case SDLK_4:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_4], IsDown, WasDown); break; 
						case SDLK_5:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_5], IsDown, WasDown); break; 
						case SDLK_6:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_6], IsDown, WasDown); break; 
						case SDLK_7:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_7], IsDown, WasDown); break; 
						case SDLK_8:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_8], IsDown, WasDown); break; 
						case SDLK_9:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_9], IsDown, WasDown); break; 
						case SDLK_a:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_A], IsDown, WasDown); break; 
						case SDLK_AC_BACK:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AC_BACK], IsDown, WasDown); break; 
						case SDLK_AC_BOOKMARKS:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AC_BOOKMARKS], IsDown, WasDown); break; 
						case SDLK_AC_FORWARD:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AC_FORWARD], IsDown, WasDown); break; 
						case SDLK_AC_HOME:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AC_HOME], IsDown, WasDown); break; 
						case SDLK_AC_REFRESH:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AC_REFRESH], IsDown, WasDown); break; 
						case SDLK_AC_SEARCH:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AC_SEARCH], IsDown, WasDown); break; 
						case SDLK_AC_STOP:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AC_STOP], IsDown, WasDown); break; 
						case SDLK_AGAIN:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AGAIN], IsDown, WasDown); break; 
						case SDLK_ALTERASE:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_ALTERASE], IsDown, WasDown); break; 
						case SDLK_QUOTE:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_QUOTE], IsDown, WasDown); break; 
						case SDLK_APPLICATION:                  AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_APPLICATION], IsDown, WasDown); break; 
						case SDLK_AUDIOMUTE:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AUDIOMUTE], IsDown, WasDown); break; 
						case SDLK_AUDIONEXT:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AUDIONEXT], IsDown, WasDown); break; 
						case SDLK_AUDIOPLAY:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AUDIOPLAY], IsDown, WasDown); break; 
						case SDLK_AUDIOPREV:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AUDIOPREV], IsDown, WasDown); break; 
						case SDLK_AUDIOSTOP:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AUDIOSTOP], IsDown, WasDown); break; 
						case SDLK_b:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_B], IsDown, WasDown); break; 
						case SDLK_BACKSLASH:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_BACKSLASH], IsDown, WasDown); break; 
						case SDLK_BACKSPACE:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_BACKSPACE], IsDown, WasDown); break; 
						case SDLK_BRIGHTNESSDOWN:               AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_BRIGHTNESSDOWN], IsDown, WasDown); break; 
						case SDLK_BRIGHTNESSUP:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_BRIGHTNESSUP], IsDown, WasDown); break; 
						case SDLK_c:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_C], IsDown, WasDown); break; 
						case SDLK_CALCULATOR:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CALCULATOR], IsDown, WasDown); break; 
						case SDLK_CANCEL:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CANCEL], IsDown, WasDown); break; 
						case SDLK_CAPSLOCK:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CAPSLOCK], IsDown, WasDown); break; 
						case SDLK_CLEAR:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CLEAR], IsDown, WasDown); break; 
						case SDLK_CLEARAGAIN:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CLEARAGAIN], IsDown, WasDown); break; 
						case SDLK_COMMA:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_COMMA], IsDown, WasDown); break; 
						case SDLK_COMPUTER:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_COMPUTER], IsDown, WasDown); break; 
						case SDLK_COPY:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_COPY], IsDown, WasDown); break; 
						case SDLK_CRSEL:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CRSEL], IsDown, WasDown); break; 
						case SDLK_CURRENCYSUBUNIT:              AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CURRENCYSUBUNIT], IsDown, WasDown); break; 
						case SDLK_CURRENCYUNIT:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CURRENCYUNIT], IsDown, WasDown); break; 
						case SDLK_CUT:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CUT], IsDown, WasDown); break; 
						case SDLK_d:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_D], IsDown, WasDown); break; 
						case SDLK_DECIMALSEPARATOR:             AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_DECIMALSEPARATOR], IsDown, WasDown); break; 
						case SDLK_DELETE:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_DELETE], IsDown, WasDown); break; 
						case SDLK_DISPLAYSWITCH:                AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_DISPLAYSWITCH], IsDown, WasDown); break; 
						case SDLK_DOWN:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_DOWN], IsDown, WasDown); break; 
						case SDLK_e:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_E], IsDown, WasDown); break; 
						case SDLK_EJECT:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_EJECT], IsDown, WasDown); break; 
						case SDLK_END:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_END], IsDown, WasDown); break; 
						case SDLK_EQUALS:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_EQUALS], IsDown, WasDown); break; 
						case SDLK_ESCAPE:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_ESC], IsDown, WasDown); break; 
						case SDLK_EXECUTE:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_EXE], IsDown, WasDown); break; 
						case SDLK_EXSEL:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_EXSEL], IsDown, WasDown); break; 
						case SDLK_f:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F], IsDown, WasDown); break; 
						case SDLK_F1:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F1], IsDown, WasDown); break; 
						case SDLK_F10:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F10], IsDown, WasDown); break; 
						case SDLK_F11:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F11], IsDown, WasDown); break; 
						case SDLK_F12:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F12], IsDown, WasDown); break; 
						case SDLK_F13:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F13], IsDown, WasDown); break; 
						case SDLK_F14:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F14], IsDown, WasDown); break; 
						case SDLK_F15:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F15], IsDown, WasDown); break; 
						case SDLK_F16:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F16], IsDown, WasDown); break; 
						case SDLK_F17:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F17], IsDown, WasDown); break; 
						case SDLK_F18:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F18], IsDown, WasDown); break; 
						case SDLK_F19:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F19], IsDown, WasDown); break; 
						case SDLK_F2:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F2], IsDown, WasDown); break; 
						case SDLK_F20:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F20], IsDown, WasDown); break; 
						case SDLK_F21:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F21], IsDown, WasDown); break; 
						case SDLK_F22:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F22], IsDown, WasDown); break; 
						case SDLK_F23:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F23], IsDown, WasDown); break; 
						case SDLK_F24:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F24], IsDown, WasDown); break; 
						case SDLK_F3:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F3], IsDown, WasDown); break; 
						case SDLK_F4:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F4], IsDown, WasDown); break; 
						case SDLK_F5:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F5], IsDown, WasDown); break; 
						case SDLK_F6:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F6], IsDown, WasDown); break; 
						case SDLK_F7:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F7], IsDown, WasDown); break; 
						case SDLK_F8:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F8], IsDown, WasDown); break; 
						case SDLK_F9:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_F9], IsDown, WasDown); break; 
						case SDLK_FIND:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_FIND], IsDown, WasDown); break; 
						case SDLK_g:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_G], IsDown, WasDown); break; 
						case SDLK_BACKQUOTE:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_BACKQUOTE], IsDown, WasDown); break; 
						case SDLK_h:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_H], IsDown, WasDown); break; 
						case SDLK_HELP:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_HELP], IsDown, WasDown); break; 
						case SDLK_HOME:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_HOME], IsDown, WasDown); break; 
						case SDLK_i:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_I], IsDown, WasDown); break; 
						case SDLK_INSERT:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_INSERT], IsDown, WasDown); break; 
						case SDLK_j:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_J], IsDown, WasDown); break; 
						case SDLK_k:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_K], IsDown, WasDown); break; 
						case SDLK_KBDILLUMDOWN:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KBDILLUMDOWN], IsDown, WasDown); break; 
						case SDLK_KBDILLUMTOGGLE:               AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KBDILLUMTOGGLE], IsDown, WasDown); break; 
						case SDLK_KBDILLUMUP:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KBDILLUMUP], IsDown, WasDown); break; 
						case SDLK_KP_0:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_0], IsDown, WasDown); break; 
						case SDLK_KP_00:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_00], IsDown, WasDown); break; 
						case SDLK_KP_000:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_000], IsDown, WasDown); break; 
						case SDLK_KP_1:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_1], IsDown, WasDown); break; 
						case SDLK_KP_2:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_2], IsDown, WasDown); break; 
						case SDLK_KP_3:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_3], IsDown, WasDown); break; 
						case SDLK_KP_4:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_4], IsDown, WasDown); break; 
						case SDLK_KP_5:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_5], IsDown, WasDown); break; 
						case SDLK_KP_6:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_6], IsDown, WasDown); break; 
						case SDLK_KP_7:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_7], IsDown, WasDown); break; 
						case SDLK_KP_8:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_8], IsDown, WasDown); break; 
						case SDLK_KP_9:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_9], IsDown, WasDown); break; 
						case SDLK_KP_A:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_A], IsDown, WasDown); break; 
						case SDLK_KP_AMPERSAND:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_AMPERSAND], IsDown, WasDown); break; 
						case SDLK_KP_AT:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_AT], IsDown, WasDown); break; 
						case SDLK_KP_B:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_B], IsDown, WasDown); break; 
						case SDLK_KP_BACKSPACE:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_BACKSPACE], IsDown, WasDown); break; 
						case SDLK_KP_BINARY:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_BINARY], IsDown, WasDown); break; 
						case SDLK_KP_C:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_C], IsDown, WasDown); break; 
						case SDLK_KP_CLEAR:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_CLEAR], IsDown, WasDown); break; 
						case SDLK_KP_CLEARENTRY:                AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_CLEARENTRY], IsDown, WasDown); break; 
						case SDLK_KP_COLON:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_COLON], IsDown, WasDown); break; 
						case SDLK_KP_COMMA:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_COMMA], IsDown, WasDown); break; 
						case SDLK_KP_D:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_D], IsDown, WasDown); break; 
						case SDLK_KP_DBLAMPERSAND:              AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_DBLAMPERSAND], IsDown, WasDown); break; 
						case SDLK_KP_DBLVERTICALBAR:            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_DBLVERTICALBAR], IsDown, WasDown); break; 
						case SDLK_KP_DECIMAL:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_DECIMAL], IsDown, WasDown); break; 
						case SDLK_KP_DIVIDE:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_DIVIDE], IsDown, WasDown); break; 
						case SDLK_KP_E:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_E], IsDown, WasDown); break; 
						case SDLK_KP_ENTER:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_ENTER], IsDown, WasDown); break; 
						case SDLK_KP_EQUALS:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_EQUALS], IsDown, WasDown); break; 
						case SDLK_KP_EQUALSAS400:               AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_EQUALSAS400], IsDown, WasDown); break; 
						case SDLK_KP_EXCLAM:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_EXCLAM], IsDown, WasDown); break; 
						case SDLK_KP_F:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_F], IsDown, WasDown); break; 
						case SDLK_KP_GREATER:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_GREATER], IsDown, WasDown); break; 
						case SDLK_KP_HASH:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_HASH], IsDown, WasDown); break; 
						case SDLK_KP_HEXADECIMAL:               AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_HEXADECIMAL], IsDown, WasDown); break; 
						case SDLK_KP_LEFTBRACE:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_LEFTBRACE], IsDown, WasDown); break; 
						case SDLK_KP_LEFTPAREN:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_LEFTPAREN], IsDown, WasDown); break; 
						case SDLK_KP_LESS:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_LESS], IsDown, WasDown); break; 
						case SDLK_KP_MEMADD:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MEMADD], IsDown, WasDown); break; 
						case SDLK_KP_MEMCLEAR:                  AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MEMCLEAR], IsDown, WasDown); break; 
						case SDLK_KP_MEMDIVIDE:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MEMDIVIDE], IsDown, WasDown); break; 
						case SDLK_KP_MEMMULTIPLY:               AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MEMMULTIPLY], IsDown, WasDown); break; 
						case SDLK_KP_MEMRECALL:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MEMRECALL], IsDown, WasDown); break; 
						case SDLK_KP_MEMSTORE:                  AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MEMSTORE], IsDown, WasDown); break; 
						case SDLK_KP_MEMSUBTRACT:               AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MEMSUBTRACT], IsDown, WasDown); break; 
						case SDLK_KP_MINUS:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MINUS], IsDown, WasDown); break; 
						case SDLK_KP_MULTIPLY:                  AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_MULTIPLY], IsDown, WasDown); break; 
						case SDLK_KP_OCTAL:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_OCTAL], IsDown, WasDown); break; 
						case SDLK_KP_PERCENT:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_PERCENT], IsDown, WasDown); break; 
						case SDLK_KP_PERIOD:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_PERIOD], IsDown, WasDown); break; 
						case SDLK_KP_PLUS:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_PLUS], IsDown, WasDown); break; 
						case SDLK_KP_PLUSMINUS:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_PLUSMINUS], IsDown, WasDown); break; 
						case SDLK_KP_POWER:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_POWER], IsDown, WasDown); break; 
						case SDLK_KP_RIGHTBRACE:                AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_RIGHTBRACE], IsDown, WasDown); break; 
						case SDLK_KP_RIGHTPAREN:                AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_RIGHTPAREN], IsDown, WasDown); break; 
						case SDLK_KP_SPACE:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_SPACE], IsDown, WasDown); break; 
						case SDLK_KP_TAB:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_TAB], IsDown, WasDown); break; 
						case SDLK_KP_VERTICALBAR:               AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_VERTICALBAR], IsDown, WasDown); break; 
						case SDLK_KP_XOR:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_KP_XOR], IsDown, WasDown); break; 
						case SDLK_l:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_L], IsDown, WasDown); break; 
						case SDLK_LALT:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_LALT], IsDown, WasDown); break; 
						case SDLK_LCTRL:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_LCTRL], IsDown, WasDown); break; 
						case SDLK_LEFT:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_LEFT], IsDown, WasDown); break; 
						case SDLK_LEFTBRACKET:                  AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_LEFTBRACKET], IsDown, WasDown); break; 
						case SDLK_LGUI:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_LGUI], IsDown, WasDown); break; 
						case SDLK_LSHIFT:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_LSHIFT], IsDown, WasDown); break; 
						case SDLK_m:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_M], IsDown, WasDown); break; 
						case SDLK_MAIL:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_MAIL], IsDown, WasDown); break; 
						case SDLK_MEDIASELECT:                  AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_MEDIASELECT], IsDown, WasDown); break; 
						case SDLK_MENU:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_MENU], IsDown, WasDown); break; 
						case SDLK_MINUS:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_MINUS], IsDown, WasDown); break; 
						case SDLK_MODE:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_MODE], IsDown, WasDown); break; 
						case SDLK_MUTE:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_MUTE], IsDown, WasDown); break; 
						case SDLK_n:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_N], IsDown, WasDown); break; 
						case SDLK_NUMLOCKCLEAR:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_NUMLOCKCLEAR], IsDown, WasDown); break; 
						case SDLK_o:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_O], IsDown, WasDown); break; 
						case SDLK_OPER:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_OPER], IsDown, WasDown); break; 
						case SDLK_OUT:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_OUT], IsDown, WasDown); break; 
						case SDLK_p:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_P], IsDown, WasDown); break; 
						case SDLK_PAGEDOWN:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PAGEDOWN], IsDown, WasDown); break; 
						case SDLK_PAGEUP:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PAGEUP], IsDown, WasDown); break; 
						case SDLK_PASTE:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PASTE], IsDown, WasDown); break; 
						case SDLK_PAUSE:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PAUSE], IsDown, WasDown); break; 
						case SDLK_PERIOD:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PERIOD], IsDown, WasDown); break; 
						case SDLK_POWER:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_POWER], IsDown, WasDown); break; 
						case SDLK_PRINTSCREEN:                  AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PRINTSCREEN], IsDown, WasDown); break; 
						case SDLK_PRIOR:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PRIOR], IsDown, WasDown); break; 
						case SDLK_q:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_Q], IsDown, WasDown); break; 
						case SDLK_r:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_R], IsDown, WasDown); break; 
						case SDLK_RALT:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_RALT], IsDown, WasDown); break; 
						case SDLK_RCTRL:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_RCTRL], IsDown, WasDown); break; 
						case SDLK_RETURN:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_ENTER], IsDown, WasDown); break; 
						case SDLK_RETURN2:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_RETURN2], IsDown, WasDown); break; 
						case SDLK_RGUI:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_RGUI], IsDown, WasDown); break; 
						case SDLK_RIGHT:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_RIGHT], IsDown, WasDown); break; 
						case SDLK_RIGHTBRACKET:                 AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_RIGHTBRACKET], IsDown, WasDown); break; 
						case SDLK_RSHIFT:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_RSHIFT], IsDown, WasDown); break; 
						case SDLK_s:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_S], IsDown, WasDown); break; 
						case SDLK_SCROLLLOCK:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_SCROLLLOCK], IsDown, WasDown); break; 
						case SDLK_SELECT:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_SELECT], IsDown, WasDown); break; 
						case SDLK_SEMICOLON:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_SEMICOLON], IsDown, WasDown); break; 
						case SDLK_SEPARATOR:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_SEPARATOR], IsDown, WasDown); break; 
						case SDLK_SLASH:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_SLASH], IsDown, WasDown); break; 
						case SDLK_SLEEP:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_SLEEP], IsDown, WasDown); break; 
						case SDLK_SPACE:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_SPACE], IsDown, WasDown); break; 
						case SDLK_STOP:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_STOP], IsDown, WasDown); break; 
						case SDLK_SYSREQ:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_SYSREQ], IsDown, WasDown); break; 
						case SDLK_t:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_T], IsDown, WasDown); break; 
						case SDLK_TAB:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_TAB], IsDown, WasDown); break; 
						case SDLK_THOUSANDSSEPARATOR:           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_THOUSANDSSEPARATOR], IsDown, WasDown); break; 
						case SDLK_u:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_U], IsDown, WasDown); break; 
						case SDLK_UNDO:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_UNDO], IsDown, WasDown); break; 
						case SDLK_UNKNOWN:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_UNKNOWN], IsDown, WasDown); break; 
						case SDLK_UP:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_UP], IsDown, WasDown); break; 
						case SDLK_v:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_V], IsDown, WasDown); break; 
						case SDLK_VOLUMEDOWN:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_VOLUMEDOWN], IsDown, WasDown); break; 
						case SDLK_VOLUMEUP:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_VOLUMEUP], IsDown, WasDown); break; 
						case SDLK_w:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_W], IsDown, WasDown); break; 
						case SDLK_WWW:                          AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_WWW], IsDown, WasDown); break; 
						case SDLK_x:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_X], IsDown, WasDown); break; 
						case SDLK_y:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_Y], IsDown, WasDown); break; 
						case SDLK_z:                            AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_Z], IsDown, WasDown); break; 
						case SDLK_AMPERSAND:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AMPERSAND], IsDown, WasDown); break; 
						case SDLK_ASTERISK:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_ASTERISK], IsDown, WasDown); break; 
						case SDLK_AT:                           AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_AT], IsDown, WasDown); break; 
						case SDLK_CARET:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_CARET], IsDown, WasDown); break; 
						case SDLK_COLON:                        AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_COLON], IsDown, WasDown); break; 
						case SDLK_DOLLAR:                       AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_DOLLAR], IsDown, WasDown); break; 
						case SDLK_EXCLAIM:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_EXCLAIM], IsDown, WasDown); break; 
						case SDLK_GREATER:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_GREATER], IsDown, WasDown); break; 
						case SDLK_HASH:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_HASH], IsDown, WasDown); break; 
						case SDLK_LEFTPAREN:                    AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_LEFTPAREN], IsDown, WasDown); break; 
						case SDLK_LESS:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_LESS], IsDown, WasDown); break; 
						case SDLK_PERCENT:                      AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PERCENT], IsDown, WasDown); break; 
						case SDLK_PLUS:                         AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_PLUS], IsDown, WasDown); break; 
						case SDLK_QUESTION:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_QUESTION], IsDown, WasDown); break; 
						case SDLK_QUOTEDBL:                     AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_QUOTEDBL], IsDown, WasDown); break; 
						case SDLK_RIGHTPAREN:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_RIGHTPAREN], IsDown, WasDown); break; 
						case SDLK_UNDERSCORE:                   AE_ProcessKeyPress(&Input->Keyboard.Keys[AEK_UNDERSCORE], IsDown, WasDown); break; 
					}
				}
			} break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			{
				bool IsDown = (Event->button.state == SDL_PRESSED);
				bool WasDown = false;
				if(Event->button.state == SDL_RELEASED)
				{
					WasDown = true;
				}
				else if(Event->key.repeat != 0)
				{
					WasDown = true;
				}
				
				switch(Event->button.button)
				{
					case SDL_BUTTON_LEFT:
					{
						AE_ProcessKeyPress(&Input->Mouse.Keys[AE_MB_LEFT], IsDown, WasDown);
					} break;
					case SDL_BUTTON_RIGHT:
					{
						AE_ProcessKeyPress(&Input->Mouse.Keys[AE_MB_RIGHT], IsDown, WasDown);
					} break;
				}				
			} break;
			case SDL_MOUSEMOTION:
			{
				SDL_GetMouseState(&Input->MouseState.x, &Input->MouseState.y);
			} break;
			case SDL_MOUSEWHEEL:
			{
				Input->Scroll.y = Event->wheel.y;
			} break;
		}
		
		Input->Scrolling = Event->type == SDL_MOUSEWHEEL;
	}
	
	return Quit;
}