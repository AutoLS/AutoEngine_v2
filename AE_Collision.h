#ifndef AE_COLLISION
#define AE_COLLISION

#include "AE_Polygon.cpp"

struct AE_collide_result
{
	bool Collide;
	v2 MTV;
	real32 Depth;
	v2 Normal;
	v2 d;
	bool Sign;
};

AE_API bool AE_TestGJK(AE_polygon* ShapeA, AE_polygon* ShapeB, 
					   AE_collide_result* CollideResult = 0);

#endif