#ifndef AE_OPENGL
#define AE_OPENGL

#include "AE_Math.h"

struct AE_GLShapeData
{
	uint32 VAO;
	uint32 VBO;
	uint32 EBO;
	
	v3* Vertices;
	uint32* Indices;
	
	uint32 nVertices;
	uint32 nVerticesTotal;
	uint32 nIndices;
	GLenum Usage;
};

AE_API AE_GLShapeData 
AE_GenShapeGL(v3* Vertices, int Count, int TotalCount, 
			  uint32* Indices = 0, int nIndices = 0, 
			  GLenum Usage = GL_STATIC_DRAW);

AE_API void AE_UploadShapeDataGL(AE_GLShapeData* Shape);
AE_API void AE_SetAttribPointerGL(AE_GLShapeData* Shape, uint32 Location, 
								  int Size, GLsizei Stride, int Offset);

struct AE_GLSimpleRenderer
{
	uint32 Shader;
	AE_GLShapeData Rect;
	AE_GLShapeData Triangle;
};

AE_API uint32 
AE_CreateShaderGL(char* VertexShaderSource, char* FragmentShaderSource);
AE_API AE_GLSimpleRenderer AE_CreateSimpleRendererGL();

AE_API uint32 
AE_LoadTextureGL(char* Path, bool Alpha = true, bool Flip = true, int n = 0);
AE_API uint32 
AE_LoadTextGL(TTF_Font* Font, const char* Text, v4 Color);

void AE_SetIntGL(uint32 Shader, char* Location, int Entry);
void AE_SetVec4GL(uint32 Shader, char* Location, v4 A);
void AE_SetMat4GL(uint32 Shader, char* Location, mat4 A, 
				  GLenum IsTranspose = GL_TRUE);
void AE_SetViewProjectionGL(uint32 Renderer, mat4 Projection, mat4 View);

AE_API void AE_DrawShapeGL(uint32 Renderer, AE_GLShapeData* Shape, 
						   v3 Pos, v3 Dim, 
						   real32 Theta, v3 Axis, v4 Color, 
						   uint32 Texture, bool Blend);
AE_API void AE_RenderClearGL(v4 Color, GLbitfield Flags);

#endif