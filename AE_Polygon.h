#ifndef AE_POLYGON
#define AE_POLYGON

enum AE_polygon_type
{
	AE_POLYGON_TRIANGLE,
	AE_POLYGON_RECTANGLE,
	AE_POLYGON_CUSTOM,
};

struct AE_polygon
{
	v2* Vertices;
	v2 Center;
	
	v2 Dim;

	int Count;
	
	real32 Theta;
	AE_polygon_type Type;
};

AE_API AE_polygon 
AE_CreatePolygon(AE_polygon_type Type, v2 Pos, v2 Dim, 
				 int Count = 0, v2* Vertices = 0);
				 
AE_API void AE_UpdateVertices(AE_polygon* Shape, v2 Delta);
AE_API void AE_RotatePolygon(AE_polygon* Shape, real32 Theta);

#endif