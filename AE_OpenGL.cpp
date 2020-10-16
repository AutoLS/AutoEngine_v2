#include "AE_OpenGL.h"

AE_GLShapeData 
AE_GenShapeGL(v3* Vertices, int Count, int TotalCount, 
			  uint32* Indices, int nIndices, GLenum Usage)
{
	AE_GLShapeData Shape = {};
	glGenVertexArrays(1, &Shape.VAO);
	glGenBuffers(1, &Shape.VBO);
	
	if(Indices)
	{
		glGenBuffers(1, &Shape.EBO);
		Shape.Indices = Indices;
		Shape.nIndices = nIndices;
	}
	
	Shape.Vertices = Vertices;
	Shape.nVertices = Count;
	Shape.nVerticesTotal = TotalCount;
	Shape.Usage = Usage;
	
	return Shape;
}

void AE_UploadShapeDataGL(AE_GLShapeData* Shape)
{
	glBindVertexArray(Shape->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Shape->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v3) * Shape->nVerticesTotal, 
				 Shape->Vertices, Shape->Usage);
	
	if(Shape->Indices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Shape->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * Shape->nIndices, 
					 Shape->Indices, Shape->Usage);
	}

	glBindVertexArray(0);
}

void AE_SetAttribPointerGL(AE_GLShapeData* Shape, uint32 Location, int Size, 
						   GLsizei Stride, int Offset)
{
	glBindVertexArray(Shape->VAO);
	glVertexAttribPointer(Location, Size, GL_FLOAT, GL_FALSE, 
						  Stride * sizeof(real32), (void*)(sizeof(real32)*Offset));
	glEnableVertexAttribArray(Location);
	glBindVertexArray(0);
}

void AE_DrawShapeGL(uint32 Renderer, AE_GLShapeData* Shape, 
						   v3 Pos, v3 Dim, 
						   real32 Theta, v3 Axis, v4 Color, 
						   uint32 Texture, bool Blend)
{	
	glUseProgram(Renderer);
	AE_SetVec4GL(Renderer, "Color", Color);
	
	mat4 Transform = Mat4();
	Transform = Scale(Transform, Dim);
	Transform = Rotate(Transform, Axis, Theta);
	Transform = Translate(Transform, Pos);
	AE_SetMat4GL(Renderer, "Transform", Transform);

	if(Blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if(Texture)
	{
		AE_SetIntGL(Renderer, "Texture", 0);
		AE_SetIntGL(Renderer, "IsTexture", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
	}
	else
	{
		AE_SetIntGL(Renderer, "IsTexture", 0);
	}
	glBindVertexArray(Shape->VAO);
	if(Shape->Indices)
	{
		glDrawElements(GL_TRIANGLES, Shape->nIndices, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, Shape->nVertices);
	}
	glDisable(GL_BLEND);
	glBindVertexArray(0);
	glUseProgram(0);
}

uint32 
AE_CreateShaderGL(char* VertexShaderSource, char* FragmentShaderSource)
{
	uint32 VertexShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderSource, 0);
	glCompileShader(VertexShader);
	
	bool32 ShaderCompileSuccess;
	char InfoLog[512];
	
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &ShaderCompileSuccess);
	if(!ShaderCompileSuccess)
	{
		glGetShaderInfoLog(VertexShader, 512, 0, InfoLog);
		printf("VERTEX SHADER COMPILATION FAILED: %s", InfoLog);
	}
	
	uint32 FragmentShader;
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, 0);
	glCompileShader(FragmentShader);
	
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &ShaderCompileSuccess);
	if(!ShaderCompileSuccess)
	{
		glGetShaderInfoLog(FragmentShader, 512, 0, InfoLog);
		printf("FRAGMENT SHADER COMPILATION FAILED: %s", InfoLog);
	}
	
	uint32 Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);
	
	bool32 LinkingSuccess;
	glGetProgramiv(Program, GL_LINK_STATUS, &LinkingSuccess);
	if(!LinkingSuccess)
	{
		glGetProgramInfoLog(Program, 512, 0, InfoLog);
		printf("PROGRAM LINKING FAILED: %s", InfoLog);
	}
	
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);  
	
	return Program;
}

AE_GLSimpleRenderer 
AE_CreateSimpleRendererGL()
{
	char* VertSource = ""  
	"#version 330 core\n" 
	"layout (location = 0) in vec3 InPos; 		\n" 
	"layout (location = 1) in vec3 InTexCoords; \n" 
	"out vec3 TexCoords;     					\n" 
	"out vec3 Vertex;        					\n" 
	"out vec3 FragPos;       					\n" 
	"uniform mat4 Transform; 					\n" 
	"uniform mat4 View;      					\n" 
	"uniform mat4 Projection;					\n" 
	"void main()											  \n" 
	"{                                                        \n" 
	"	FragPos = vec3(Transform * vec4(InPos, 1.0));         \n" 
	"	gl_Position = Projection * View * vec4(FragPos, 1.0); \n" 
	"	TexCoords = InTexCoords;                              \n" 
	"}	                                                      ";
	
	char* FragSource = "" 
	"#version 330 core\n" 
	"out vec4 FragColor;\n" 
	"in vec3 TexCoords;\n" 
	"in vec3 FragPos;\n" 
	"uniform vec4 Color;\n" 
	"uniform sampler2D Texture;\n" 
	"uniform int IsTexture;\n"
	"void main()\n" 
	"{\n" 
		"if(IsTexture > 0)\n"
		"{\n"
			"vec4 TexColor = texture(Texture, TexCoords.xy) * Color;\n" 
			"if(TexColor.a < 0.1)\n" 
				"discard;\n" 
			"FragColor = TexColor;\n" 
		"}\n"
		"else\n"
		"{\n"
			"FragColor = Color;\n"
		"}\n"
	"}";
	
	AE_GLSimpleRenderer Renderer = {};
	Renderer.Shader = AE_CreateShaderGL(VertSource, FragSource);
	
	v3 RectObjectVert[] =
	{
		V3(0.5f, 0.5f),   V3(1, 1),	//Top Right		
		V3(0.5f, -0.5f),  V3(1, 0),	//Bottom Right
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(-0.5f, 0.5f),  V3(0, 1),	//Top Left
		V3(0.5f, 0.5f),   V3(1, 1)	//Top Right	
	};
	
	v3 TriObjectVert[] =
	{
		V3(0, 0.5f),  V3(0.5, 1),	//Top Centre
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(0.5f, -0.5f),  V3(1, 0),	//Bottom Right
	};
	
	Renderer.Rect = AE_GenShapeGL(RectObjectVert, 6, ArraySize(RectObjectVert));
	AE_UploadShapeDataGL(&Renderer.Rect);
	AE_SetAttribPointerGL(&Renderer.Rect, 0, 3, 6, 0);
	AE_SetAttribPointerGL(&Renderer.Rect, 1, 3, 6, 3);
	
	Renderer.Triangle = 
	AE_GenShapeGL(TriObjectVert, 3, ArraySize(TriObjectVert));
	AE_UploadShapeDataGL(&Renderer.Triangle);
	AE_SetAttribPointerGL(&Renderer.Triangle, 0, 3, 6, 0);
	AE_SetAttribPointerGL(&Renderer.Triangle, 1, 3, 6, 3);
	
	return Renderer;
}

uint32 AE_LoadTextureGL(char* Path, bool Alpha, bool Flip, int n)
{
	stbi_set_flip_vertically_on_load(Flip);
    uint32 TextureID;
    glGenTextures(1, &TextureID);
    
    int Width, Height, Components;
    uint8 *Data = stbi_load(Path, &Width, &Height, &Components, n);
    if (Data)
    {
        GLenum Format;
        if (Components == 1)
            Format = GL_RED;
        else if (Components == 3)
            Format = GL_RGB;
        else if (Components == 4)
            Format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, 
					 GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
		if(Alpha)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		
		glTexParameteri(GL_TEXTURE_2D, 
						GL_TEXTURE_MIN_FILTER, 
						GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(Data);
    }
    else
    {
        printf("Texture failed to load at path: %s\n", Path);
        stbi_image_free(Data);
    }

    return TextureID;
}

uint32 AE_LoadTextGL(TTF_Font* Font, const char* Text, v4 Color)
{
	uint32 ID;
	glGenTextures(1, &ID);
	
	SDL_Surface* Surface;
	SDL_Color TextColor = 
	{
		RoundReal32ToUint8(Color.r * 255),
		RoundReal32ToUint8(Color.g * 255),
		RoundReal32ToUint8(Color.b * 255),
		RoundReal32ToUint8(Color.a * 255),
	};
	Surface = TTF_RenderText_Solid(Font, Text, TextColor);
		
	if(Surface)
	{
		uint8 BytesPerPixel = Surface->format->BytesPerPixel;
		GLenum Format = (BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		
		glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, 
					 Surface->w, Surface->h, 0, Format, 
				 	 GL_UNSIGNED_BYTE, Surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		printf("Failed to load text\n");
	}
	SDL_FreeSurface(Surface);
	return ID;
}

void AE_SetIntGL(uint32 Shader, char* Location, int Entry)
{
	glUseProgram(Shader);
	glUniform1i(glGetUniformLocation(Shader, Location), Entry);
}

void AE_SetVec4GL(uint32 Shader, char* Location, v4 A)
{
	glUseProgram(Shader);
	glUniform4f(glGetUniformLocation(Shader, Location), A.x, A.y, A.z, A.w);
}

void AE_SetMat4GL(uint32 Shader, char* Location, mat4 A, GLenum IsTranspose)
{
	glUseProgram(Shader);
	glUniformMatrix4fv(glGetUniformLocation(Shader, Location), 
					   1, IsTranspose, (real32*)A.E);
}

void AE_SetViewProjectionGL(uint32 Renderer, mat4 Projection, mat4 View)
{
	AE_SetMat4GL(Renderer, "Projection", Projection);
	AE_SetMat4GL(Renderer, "View", View);
}

void AE_RenderClearGL(v4 Color, GLbitfield Flags)
{
	glClearColor(Color.r, Color.g, Color.b, Color.a);
	glClear(Flags);
}