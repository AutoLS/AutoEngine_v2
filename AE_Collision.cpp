#include "AE_Collision.h"

//GJK Algorithm
int AE_FurthestPointInDirection(AE_polygon* Shape, v2 Dir)
{
	real32 Max = -FLT_MAX;
	int Result = 0;
	for(int i = 0; i < Shape->Count; ++i)
	{
		real32 DotProduct = Dot(Shape->Vertices[i], Dir);
		if(DotProduct > Max)
		{
			Max = DotProduct;
			Result = i;
		}
	}
	return Result;
}

v2 AE_SupportGJK(AE_polygon* ShapeA, AE_polygon* ShapeB, v2 Dir)
{
	int IndexA = AE_FurthestPointInDirection(ShapeA, Dir);
	int IndexB = AE_FurthestPointInDirection(ShapeB, -Dir);
	v2 p1 = ShapeA->Vertices[IndexA];
	v2 p2 = ShapeB->Vertices[IndexB];
	
	v2 Result = p1 - p2;
	
	return Result;
}

bool AE_TestGJK(AE_polygon* ShapeA, AE_polygon* ShapeB, AE_collide_result* CollideResult)
{
	int Index = 0;
	real32 Min = FLT_MAX;
	
	v2 Simplex[3] = {};
	
	v2 d = ShapeB->Center - ShapeA->Center;
	
	Simplex[Index] = AE_SupportGJK(ShapeA, ShapeB, d);
	
	v2 ClosestPoint = Simplex[Index];
	real32 Distance = Length(Simplex[Index]);
	if(Distance < Min)
	{
		Min = Distance;
	}
	
	v2 a = Simplex[0];
	if(Dot(a, d) <= 0)
	{
		if(CollideResult)
		{
			CollideResult->Collide = false;
		}
		return false;
	}
	d = -a;
	
	v2 b, c, ao, ab, ac, ab_perp, ac_perp;
	
	while(true)
	{
		a = Simplex[++Index] = AE_SupportGJK(ShapeA, ShapeB, d);
		
		Distance = Length(a);
		if(Distance < Min)
		{
			Min = Distance;
			ClosestPoint = a;
		}
		
		if(Dot(a, d) <= 0)
		{
			if(CollideResult)
			{
				CollideResult->Collide = false;
			}
			return false;
		}
		
		ao = -a;
		
		if(Index < 2)
		{
			b = Simplex[0];
			ab = b - a;
			d = TripleProduct(ab, ao, ab);
			if(d.x * d.x + d.y * d.y == 0)
			{
				d = Perp_v2(ab);
			}
			continue;
		}
		
		b = Simplex[1];
        c = Simplex[0];
        ab = b - a; // from point A to B
        ac = c - a; // from point A to C
        
        ac_perp = TripleProduct(ab, ac, ac);
        
        if (Dot(ac_perp, ao) >= 0) 
		{
            d = ac_perp; // new direction is normal to AC towards Origin
        } 
		else 
		{
            ab_perp = TripleProduct(ac, ab, ab);
            
            if (Dot(ab_perp, ao) < 0)
			{
				if(CollideResult)
				{
					CollideResult->Collide = true;
					CollideResult->MTV = V2() - ClosestPoint;
					CollideResult->Depth = Min;
				}
                return true; // collision
			}
            
            Simplex[0] = Simplex[1]; // swap first element (point C)

            d = ab_perp; // new direction is normal to AB towards Origin
        }
        
        Simplex[1] = Simplex[2]; // swap element in the middle (point B)
        --Index;
	}
	
	return false;
}