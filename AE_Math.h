#ifndef AE_MATH
#define AE_MATH

#define PI 3.1415926f

real32 Cos(real32 Theta)
{
	return (real32)cos((double)Theta);
}

real32 Sin(real32 Theta)
{
	return (real32)sin((double)Theta);
}

real32 Radians(real32 Theta)
{
	real32 Result = (real32)(Theta * (PI/180));
	return Result;
}

union v2 
{
	struct 
	{
		real32 x, y;
	};
	real32 E[2];
};

struct v2i
{
	int x, y;
};

inline v2 V2(real32 x = 0, real32 y = 0)
{
	v2 Result = {x, y};
	return Result;
}

inline v2 V2(v2i A)
{
	v2 Result = {(real32)A.x, (real32)A.y};
	return Result;
}

inline v2 IV2()
{
	v2 Result = V2(1, 1);
	return Result;
}
	
inline v2i V2i(int x = 0, int y = 0)
{
	v2i Result = {x, y};
	return Result;
}

inline v2 operator+(v2 A, v2 B)
{
	v2 Result = {A.x + B.x, A.y + B.y};
	return Result;
}

inline v2 operator+(v2 A, real32 N)
{
	v2 Result = {A.x + N, A.y + N};
	return Result;
}

inline v2 operator-(v2 A, v2 B)
{
	v2 Result = {A.x - B.x, A.y - B.y};
	return Result;
}

inline v2 operator-(v2 A)
{
	v2 Result = {-A.x, -A.y};
	return Result;
}

inline v2 operator*(v2 A, real32 k)
{
	v2 Result = {k * A.x, k * A.y};
	return Result;
}

inline v2 operator*(real32 k, v2 A)
{
	v2 Result = {k * A.x, k * A.y};
	return Result;
}

inline v2 operator*(v2 A, v2 B)
{
	v2 Result = {B.x * A.x, B.y * A.y};
	return Result;
}

inline v2 operator/(v2 A, real32 k)
{
	real32 inv = 1/k;
	v2 Result = {A.x * inv, A.y * inv};
	return Result;
}

inline v2 operator/(v2 A, v2 B)
{
	v2 Result = {A.x/B.x, A.y/B.y};
	return Result;
}

inline v2 &operator+=(v2 &A, v2 B)
{
	A = A + B;
	return A;
}

inline v2 &operator+=(v2 &A, real32 N)
{
	A = A + N;
	return A;
}

inline v2 &operator-=(v2 &A, v2 B)
{
	A = A - B;
	return A;
}

inline v2i operator-(v2i A, v2i B)
{
	v2i Result = {A.x - B.x, A.y - B.y};
	return Result;
}

inline bool 
operator==(v2 A, v2 B)
{
	return (A.x == B.x && A.y == B.y);
}

inline bool 
operator!=(v2 A, v2 B)
{
	return A.x != B.x || A.y != B.y;
}

inline real32 Length(v2 A)
{
	real32 Result = (real32)sqrt((A.x * A.x) + (A.y * A.y));
	return Result;
}

inline v2 Perp_v2(v2 A)
{
	v2 Result = {-A.y, A.x};
	return Result;
}

inline real32 Dot(v2 A, v2 B)
{
	real32 Result = {A.x*B.x + A.y*B.y};
	return Result;
}

inline v2 Normalize(v2 A)
{
	v2 Result = {};
	if(Length(A) == 0)
	{
		return V2();
	}
	else
		Result = A / Length(A);
	return Result;
}

inline v2 Project(v2 A, v2 B)
{
	v2 Result = Dot(A, B) * Normalize(B); //Project A onto B;
	return Result;
}

union v3
{
	struct
	{
		real32 x, y, z;
	};
	real32 E[3];
};

inline v3 
V3(real32 x = 0, real32 y = 0, real32 z = 0)
{
	v3 Ret = {x, y, z};
	return Ret;
}

inline v3 
IV3()
{
	v3 Result = {1, 1, 1};
	return Result;
}

inline v3 V3(v2i A, real32 z = 0)
{
	v3 Result = {(real32)A.x, (real32)A.y, z};
	return Result;
}

inline v3 V3(v2 A, real32 z = 0)
{
	v3 Result = {A.x, A.y, z};
	return Result;
}

inline bool 
operator==(v3 A, v3 B)
{
	return (A.x == B.x && A.y == B.y && A.z == B.z);
}

inline bool 
operator!=(v3 A, v3 B)
{
	return A.x != B.x || A.y != B.y || A.z != B.z;
}

inline v3 
operator-(v3 A)
{
	v3 Ret = V3(-A.x, -A.y, -A.z);
	return Ret;
}

inline v3 
operator+(v3 A, v3 B)
{
	v3 Ret = V3(A.x + B.x, A.y + B.y, A.z + B.z);
	return Ret;
}

inline v3
operator-(v3 A, v3 B)
{
	v3 Ret = A + (-B);
	return Ret;
}

inline v3
operator*(v3 A, real32 k)
{
	v3 Ret = V3(A.x * k, A.y * k, A.z * k);
	return Ret;
}

inline v3
operator*(real32 k, v3 A)
{
	v3 Ret = V3(A.x * k, A.y * k, A.z * k);
	return Ret;
}

inline v3
operator/(v3 A, real32 k)
{
	Assert(k != 0);
	
	real32 OneOverK = 1.0f / k;
	v3 Ret = A * OneOverK;
	return Ret;
}

inline v3& 
operator+=(v3 &A, v3 B)
{
	A = A + B;
	return A;
}

inline v3&
operator-=(v3 &A, v3 B)
{
	A = A - B;
	return A;
}

inline v3 operator*(v3 A, v3 B)
{
	v3 Result = {A.x * B.x, A.y * B.y, A.z * B.z};
	return Result;
}

inline v3 operator/(v3 A, v3 B)
{
	v3 Result = {A.x / B.x, A.y / B.y, A.z / B.z};
	return Result;
}

inline real32 
Length(v3 A)
{
	real32 Ret = (real32)sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
	return Ret;
}

inline real32 Distance(v3 A, v3 B)
{
	real32 Ret = Length(A - B);
	return Ret;
}

inline v3 
Normalize(v3 A)
{
	Assert(A != V3());
	v3 Ret = A / Length(A);
	return Ret;
}

inline real32 
Dot(v3 A, v3 B)
{
	real32 Ret = A.x*B.x + A.y*B.y + A.z*B.z;
	return Ret;
}

inline v3
Cross(v3 A, v3 B)
{
	v3 Ret = V3(A.z*B.y - A.y*B.z, 
				A.z*B.x - A.x*B.z, 
				A.x*B.y - A.y*B.x);
	return Ret;
}

union v4 
{
	struct
	{
		real32 x, y, z, w;
	};
	struct
	{
		real32 r, g, b, a;
	};
};

inline v4 V4(real32 x = 0, real32 y = 0, real32 z = 0, real32 w = 1)
{
	v4 Result = {x, y, z, w};
	return Result;
}

inline v4 Color(real32 r = 1, real32 g = 1, real32 b = 1, real32 a = 1)
{
	real32 OneOver255 = 1.0f / 255.0f;
	
	if(r > 1) r *= OneOver255;
	if(g > 1) g *= OneOver255;
	if(b > 1) b *= OneOver255;
	if(a > 1) a *= OneOver255;
	
	v4 Result = {r, g, b, a};
	return Result;
}

//IMPORTANT: Matrices are construct in row major! Which means that each basis vector of a coordinate space are store in rows!
union mat4
{
	struct
	{
		real32 E[4][4];
	};
	
	struct 
	{
		real32 m[16];
	};
};

mat4 Mat4()
{
	mat4 Result = {};
	for(int r = 0; r < 4; ++r)
	{
		Result.E[r][r] = 1;
	}
	
	return Result;
}

#if 0
mat4 Mat4Rand()
{
	mat4 Result;
	for(int r = 0; r < 4; ++r)
	{
		for(int c = 0; c < 4; ++c)
		{
			Result.E[r][c] = Rand32(0, 100);
		}
	}
	
	return Result;
}
#endif

mat4 operator*(mat4 A, mat4 B)
{
	mat4 Result = {};
	for(int r = 0; r < 4; ++r)
	{
		for(int c = 0; c < 4; ++c)
		{
			Result.E[r][c] = A.E[r][0] * B.E[0][c] +
							 A.E[r][1] * B.E[1][c] +
							 A.E[r][2] * B.E[2][c] +
							 A.E[r][3] * B.E[3][c];
		}
	}
	return Result;
}

v4 operator*(mat4 &A, v4 B)
{
	v4 Result = {};
	real32* PtrResult = &Result.x;
	for(int i = 0; i < 4; ++i)
	{
		*(PtrResult + i) = A.E[i][0] * B.x +
						   A.E[i][1] * B.y +
						   A.E[i][2] * B.z +
						   A.E[i][3] * B.w;
	}
	return Result;
}

v4 operator*(v4 B, mat4 &A)
{
	v4 Result = {};
	real32* PtrResult = &Result.x;
	for(int i = 0; i < 4; ++i)
	{
		*(PtrResult + i) = A.E[i][0] * B.x +
						   A.E[i][1] * B.y +
						   A.E[i][2] * B.z +
						   A.E[i][3] * B.w;
	}
	return Result;
}

mat4 Transpose(mat4 A)
{
	mat4 Result = Mat4();
	
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			Result.E[i][j] = A.E[j][i];
		}
	}
	
	return Result;
}

mat4 Scale(mat4& A, v3 K)
{
	mat4 Result = A;
	real32* Ptr = &K.x;
	for(int r = 0; r < 3; ++r)
	{
		Result.E[r][r] *= *(Ptr + r);
	}
	
	return Result;
}

mat4 ArbitraryAxisScale(v3 Axis, real32 k)
{
	mat4 Result = Mat4();
	
	v3* p = (v3*)&Result.E[0][0];
	p->x = 1 + ((k-1)*Axis.x*Axis.x);
	p->y = (k-1)*Axis.x*Axis.y;
	p->z = (k-1)*Axis.x*Axis.z;
	
	v3* q = (v3*)&Result.E[1][0];
	q->x = (k-1)*Axis.x*Axis.y;
	q->y = 1 + ((k-1)*Axis.y*Axis.y);
	q->z = ((k-1)*Axis.y*Axis.z);
	
	v3* r = (v3*)&Result.E[2][0];
	r->x = (k-1)*Axis.x*Axis.z;
	r->y = (k-1)*Axis.y*Axis.z;
	r->z = 1 + ((k-1)*Axis.z*Axis.z);
	
	return Result;
}

mat4 Translate(mat4 A, v3 T)
{
	mat4 Result = A;
	
	Result.E[0][3] += T.x;
	Result.E[1][3] += T.y;
	Result.E[2][3] += T.z;

	return Result;
}

mat4 Rotate(mat4 A, v3 Axis, real32 Theta)
{
	if(Length(Axis))
	Axis = Normalize(Axis);
	
	mat4 Result = Mat4();
	
	//p'
	Result.E[0][0] = cosf(Theta) + ((Axis.x * Axis.x) * (1 - cosf(Theta)));
	Result.E[0][1] = (Axis.x * Axis.y * (1 - cosf(Theta))) - (Axis.z * sinf(Theta));
	Result.E[0][2] = (Axis.x * Axis.z * (1 - cosf(Theta))) + (Axis.y * sinf(Theta));
	
	//q'
	Result.E[1][0] = (Axis.y * Axis.x * (1 - cosf(Theta))) + (Axis.z * sinf(Theta));
	Result.E[1][1] = cosf(Theta) + ((Axis.y * Axis.y) * (1 - cosf(Theta)));
	Result.E[1][2] = (Axis.z * Axis.y * (1 - cosf(Theta))) - (Axis.x * sinf(Theta));
	
	//r'
	Result.E[2][0] = (Axis.z * Axis.x * (1 - cosf(Theta))) - (Axis.y * sinf(Theta));
	Result.E[2][1] = (Axis.z * Axis.y * (1 - cosf(Theta))) + (Axis.x * sinf(Theta));
	Result.E[2][2] = cosf(Theta) + ((Axis.z * Axis.z) * (1 - cosf(Theta)));
	
	Result = Result * A;
	
	return Result;
}

mat4 RotateZ(mat4& A, real32 Theta)
{
	mat4 Result = Mat4();
	Result.E[0][0] = Cos(Theta);
	Result.E[0][1] = -Sin(Theta);
	Result.E[1][0] = Sin(Theta);
	Result.E[1][1] = Cos(Theta);
	
	return A * Result;
}

mat4 Ortho(real32 Left, real32 Right, 
		   real32 Bottom, real32 Top, 
		   real32 Near, real32 Far)
{
	mat4 Result = Mat4();
	Result.E[0][0] = 2 / (Right - Left);
	Result.E[1][1] = 2 / (Top - Bottom);
	Result.E[2][2] = -2 / (Far - Near);
	
	Result.E[0][3] = -(Right + Left) / (Right - Left);
	Result.E[1][3] = -(Top + Bottom) / (Top - Bottom);
	Result.E[2][3] = -(Far + Near) / (Far - Near);
	
	return Result;
}

mat4 Perspective(real32 FOV, real32 Aspect, real32 Near, real32 Far)
{
	mat4 Result = Mat4();
	
	real32 Top = Near * (real32)tan(Radians(FOV)/2);
	real32 Bottom = -Top;
	real32 Right = Top * Aspect;
	real32 Left = -Right;
	
	Result.E[0][0] = 2 * Near/(Right - Left);
	Result.E[1][1] = 2 * Near/(Top - Bottom);
	Result.E[2][2] = -(Far + Near)/(Far - Near);
	
	Result.E[0][3] = -Near * (Right + Left) / (Right - Left);
	Result.E[1][3] = -Near * (Top + Bottom) / (Top - Bottom);
	Result.E[2][3] = 2 * Far * Near / (Near - Far);
	
	Result.E[3][2] = -1;
	
	return Result;
}

mat4 LookAt(v3 Pos, v3 Target, v3 Up)
{
	v3 Dir = Normalize(Pos - Target);
	v3 Right = Normalize(Cross(Up, Dir));
	v3 CameraUp = Cross(Dir, Right);
	
	mat4 Rotation = Mat4();
	Rotation.E[0][0] = Right.x;
	Rotation.E[0][1] = Right.y;
	Rotation.E[0][2] = Right.z;
	
	Rotation.E[1][0] = CameraUp.x;
	Rotation.E[1][1] = CameraUp.y;
	Rotation.E[1][2] = CameraUp.z;
	
	Rotation.E[2][0] = Dir.x;
	Rotation.E[2][1] = Dir.y;
	Rotation.E[2][2] = Dir.z;
	
	mat4 Translation = Mat4();
	Translation = Translate(Translation, -Pos);
	
	return Rotation * Translation;
}

enum rect_position
{
	POSITION_TOP_LEFT,
	POSITION_TOP_RIGHT,
	POSITION_BOTTOM_LEFT,
	POSITION_BOTTOM_RIGHT,
	POSITION_CENTERED,
	POSITION_CUSTOM
};

struct rect32
{
	v2 Pos;
	v2 Dim;
};

inline rect32 Rect32(v2 Pos = {}, v2 Dim = {})
{
	rect32 Result = {Pos, Dim};
	return Result;
}

void SetRect32ScreenSpace(rect32* src, rect32* dst, v2 Offset, 
						  rect_position Position)
{
	switch(Position)
	{
		case POSITION_CENTERED:
		{
			src->Pos.x = dst->Pos.x + (dst->Dim.x*0.5f) + Offset.x - 
						 (src->Dim.x*0.5f);
			src->Pos.y = dst->Pos.y + (dst->Dim.y*0.5f) + Offset.y - 
						 (src->Dim.y*0.5f);
		} break;
		case POSITION_TOP_RIGHT:
		{
			src->Pos.x = dst->Pos.x + dst->Dim.x + Offset.x - src->Dim.x;
			src->Pos.y = dst->Pos.y + Offset.y;
		} break;
		case POSITION_TOP_LEFT:
		{
			src->Pos.x = dst->Pos.x + Offset.x;
			src->Pos.y = dst->Pos.y + Offset.y;
		} break;
		case POSITION_BOTTOM_LEFT:
		{
			src->Pos.x = dst->Pos.x + Offset.x;
			src->Pos.y = dst->Pos.y + dst->Dim.y + Offset.y - src->Dim.y;
		} break;
		case POSITION_BOTTOM_RIGHT:
		{
			src->Pos.x = dst->Pos.x + dst->Dim.x + Offset.x - src->Dim.x;
			src->Pos.y = dst->Pos.y + dst->Dim.y + Offset.y - src->Dim.y;
		} break;
	}
}

bool IsPointInRect(v2 Point, rect32* Rect)
{
	if(Point.x < Rect->Pos.x)
	{
		return false;
	}
	if(Point.x > Rect->Pos.x + Rect->Dim.x)
	{
		return false;
	}
	if(Point.y < Rect->Pos.y)
	{
		return false;
	}
	if(Point.y > Rect->Pos.y + Rect->Dim.y)
	{
		return false;
	}
	return true;
}

inline uint8 RoundReal32ToUint8(real32 n)
{
	return (uint8)(n + 0.5f);
}

inline int32 RoundReal32ToInt32(real32 n)
{
	return (int32)(n + 0.5f);
}

inline SDL_Rect ToSDLRect(rect32* Rect)
{
	SDL_Rect Ret = {
		RoundReal32ToInt32(Rect->Pos.x),
		RoundReal32ToInt32(Rect->Pos.y),
		RoundReal32ToInt32(Rect->Dim.x),
		RoundReal32ToInt32(Rect->Dim.y)
	};
	
	return Ret;
}

inline SDL_Color ToSDLColor(v4 Src)
{
	SDL_Color Ret = {
		RoundReal32ToUint8(Src.r * 255.0f),
		RoundReal32ToUint8(Src.g * 255.0f),
		RoundReal32ToUint8(Src.b * 255.0f),
		RoundReal32ToUint8(Src.a * 255.0f)
	};
	
	return Ret;
}

#endif