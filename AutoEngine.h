#ifndef AUTOENGINE_H
#define AUTOENGINE_H

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_TTF.h>
#include <SDL_Mixer.h>
#include <SDL_Net.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stb_image.cpp"

#define global_variable static;
#define ArraySize(arr) (sizeof(arr)/sizeof(arr[0]))
#define Assert(Expression) if(!(Expression)) {*(int*)0 = 0;}

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;
typedef float real32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#define AE_API extern "C" __declspec(dllexport)

#if COMPILING_THE_DLL
	#define AE_GLOBAL_VARIABLE extern "C" __declspec(dllexport)
#else
	#define AE_GLOBAL_VARIABLE extern "C" __declspec(dllimport)
#endif

enum AE_LibType
{
	AE_LIB_SDL = 0x1,
	AE_LIB_SDL_FULL = 0x2,
	AE_LIB_OPENGL = 0x4,
};

enum AE_ErrorCode
{
	AE_ERR_NONE,
	AE_LIB_ERR_FLAG_IS_ZERO,
	AE_LIB_ERR_SDL_INIT_FAILURE,
	AE_LIB_ERR_SDL_IMG_FAILURE,
	AE_LIB_ERR_SDL_MIX_FAILURE,
	AE_LIB_ERR_SDL_TTF_FAILURE,
	AE_LIB_ERR_SDL_NET_FAILURE,
	AE_LIB_ERR_GLEW_LOAD_FAILURE,
	AE_ERR_CREATE_WINDOW_FAILURE,
	AE_ERR_CREATE_RENDERER_FAILURE,
	AE_ERR_GL_CREATE_CONTEXT_FAILURE,
	AE_ERR_GL_MAKE_CURRENT_CONTEXT_FAILURE,
};

AE_GLOBAL_VARIABLE bool32 AE_GlobalLastError;
void AE_SetLastError(AE_ErrorCode Code) { AE_GlobalLastError = Code; }
bool32 AE_GetLastError() { return AE_GlobalLastError; }

AE_API void AE_PrintLastError();
AE_API bool32 AE_InitLib(uint8 Flags);

#include "AE_OpenGL.cpp"
AE_GLOBAL_VARIABLE bool32 AE_IsOpenGL;

#include "AE_Math.h"
#include "AE_Time.cpp"
#include "AE_Graphics.cpp"
#include "AE_Render.cpp"
#include "AE_Input.cpp"

struct AE_Core
{
	AE_Graphics* Graphics;
	
	SDL_Event Event;
	AE_Input Input;
	AE_Clock Clock;
};

AE_API AE_Core AE_InitCore(AE_Graphics* Graphics);
AE_API bool AE_HandleEvents(AE_Core* Core);

#include "AE_UI.cpp"

#endif