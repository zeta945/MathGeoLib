#include <stdio.h>
#include <stdlib.h>

#include "MathGeoLib.h"
#include "myassert.h"
#include "TestRunner.h"

AABB RandomAABBContainingPoint(const float3 &pt, float maxSideLength)
{
	float w = rng.Float(1e-2f, maxSideLength);
	float h = rng.Float(1e-2f, maxSideLength);
	float d = rng.Float(1e-2f, maxSideLength);

	AABB a(float3(0,0,0), float3(w,h,d));
	w = rng.Float(1e-3f, w-1e-3f);
	h = rng.Float(1e-3f, h-1e-3f);
	d = rng.Float(1e-3f, d-1e-3f);
	a.Translate(pt - float3(w,h,d));
	assert(!a.IsDegenerate());
	assert(a.IsFinite());
	assert(a.Contains(pt));
	return a;
}

OBB RandomOBBContainingPoint(const float3 &pt, float maxSideLength)
{
	AABB a = RandomAABBContainingPoint(pt, maxSideLength);
	float3x4 rot = float3x4::RandomRotation(rng);
	float3x4 tm = float3x4::Translate(pt) * rot * float3x4::Translate(-pt);
	OBB o = a.Transform(tm);
	assert(!o.IsDegenerate());
	assert(o.IsFinite());
	assert(o.Contains(pt));
	return o;
}

Sphere RandomSphereContainingPoint(const float3 &pt, float maxRadius)
{
	Sphere s(pt, rng.Float(1.f, maxRadius));
	s.pos += float3::RandomSphere(rng, float3::zero, Max(0.f, s.r - 1e-2f));
	assert(s.IsFinite());
	assert(!s.IsDegenerate());
	assert(s.Contains(pt));
	return s;
}

Frustum RandomFrustumContainingPoint(const float3 &pt)
{
	Frustum f;
	if (rng.Int(0,1))
	{
		f.type = OrthographicFrustum;
		f.orthographicWidth = rng.Float(0.001f, SCALE);
		f.orthographicHeight = rng.Float(0.001f, SCALE);
	}
	else
	{
		f.type = PerspectiveFrustum;
		// Really random Frustum could have fov as ]0, pi[, but limit
		// to much narrower fovs to not cause the corner vertices
		// shoot too far when farPlaneDistance is very large.
		f.horizontalFov = rng.Float(0.001f, 3.f*pi/4.f);
		f.verticalFov = rng.Float(0.001f, 3.f*pi/4.f);
	}
	f.nearPlaneDistance = rng.Float(0.1f, SCALE);
	f.farPlaneDistance = f.nearPlaneDistance + rng.Float(0.1f, SCALE);
	f.pos = float3::zero;
	f.front = float3::RandomDir(rng);
	f.up = f.front.RandomPerpendicular(rng);

	float3 pt2 = f.UniformRandomPointInside(rng);
	f.pos += pt - pt2;

	assert(f.IsFinite());
//	assert(!f.IsDegenerate());
	assert(f.Contains(pt));
	return f;
}

Line RandomLineContainingPoint(const float3 &pt)
{
	float3 dir = float3::RandomDir(rng);
	Line l(pt, dir);
	l.pos = l.GetPoint(rng.Float(-SCALE, SCALE));
	assert(l.IsFinite());
	assert(l.Contains(pt));
	return l;
}

Ray RandomRayContainingPoint(const float3 &pt)
{
	float3 dir = float3::RandomDir(rng);
	Ray l(pt, dir);
	l.pos = l.GetPoint(rng.Float(-SCALE, 0));
	assert(l.IsFinite());
	assert(l.Contains(pt));
	return l;
}

LineSegment RandomLineSegmentContainingPoint(const float3 &pt)
{
	float3 dir = float3::RandomDir(rng);
	float a = rng.Float(0, SCALE);
	float b = rng.Float(0, SCALE);
	LineSegment l(pt + a*dir, pt - b*dir);
	assert(l.IsFinite());
	assert(l.Contains(pt));
	return l;
}

Capsule RandomCapsuleContainingPoint(const float3 &pt)
{
	float3 dir = float3::RandomDir(rng);
	float a = rng.Float(0, SCALE);
	float b = rng.Float(0, SCALE);
	float r = rng.Float(0.001f, SCALE);
	Capsule c(pt + a*dir, pt - b*dir, r);
	float3 d = float3::RandomSphere(rng, float3::zero, c.r);
	c.l.a += d;
	c.l.b += d;
	assert(c.IsFinite());
	assert(c.Contains(pt));

	return c;
}

Plane RandomPlaneContainingPoint(const float3 &pt)
{
	float3 dir = float3::RandomDir(rng);
	Plane p(pt, dir);
	assert(!p.IsDegenerate());
	return p;
}

Triangle RandomTriangleContainingPoint(const float3 &pt)
{
	Plane p = RandomPlaneContainingPoint(pt);
	float3 a = pt;
	float3 b = p.Point(rng.Float(-SCALE, SCALE), rng.Float(-SCALE, SCALE));
	float3 c = p.Point(rng.Float(-SCALE, SCALE), rng.Float(-SCALE, SCALE));
	Triangle t(a,b,c);
	assert(t.Contains(pt));

	float3 d = t.RandomPointInside(rng);
	assert(t.Contains(d));
	d = d - t.a;
	t.a -= d;
	t.b -= d;
	t.c -= d;

	assert(t.IsFinite());
	assert(!t.IsDegenerate());
	assert(t.Contains(pt));
	return t;
}

Polyhedron RandomPolyhedronContainingPoint(const float3 &pt)
{
	switch(rng.Int(0,7))
	{
	case 0: return RandomAABBContainingPoint(pt, SCALE).ToPolyhedron();
	case 1: return RandomOBBContainingPoint(pt, SCALE).ToPolyhedron();
	case 2: return RandomFrustumContainingPoint(pt).ToPolyhedron();
	case 3: return Polyhedron::Tetrahedron(pt, SCALE); break;
	case 4: return Polyhedron::Octahedron(pt, SCALE); break;
	case 5: return Polyhedron::Hexahedron(pt, SCALE); break;
	case 6: return Polyhedron::Icosahedron(pt, SCALE); break;
	default: return Polyhedron::Dodecahedron(pt, SCALE); break;
	}

//	assert(t.IsFinite());
//	assert(!t.IsDegenerate());
//	assert(t.Contains(pt));
}

Polygon RandomPolygonContainingPoint(const float3 &pt)
{
	Polyhedron p = RandomPolyhedronContainingPoint(pt);
	Polygon poly = p.FacePolygon(rng.Int(0, p.NumFaces()-1));

	float3 pt2 = poly.FastRandomPointInside(rng);
	assert(poly.Contains(pt2));
	poly.Translate(pt - pt2);

	assert(!poly.IsDegenerate());
	assert(!poly.IsNull());
	assert(poly.IsPlanar());
	assert(poly.IsFinite());
	assert(poly.Contains(pt));

	return poly;
}

void TestAABBAABBIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	AABB b = RandomAABBContainingPoint(pt, 10.f);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBOBBIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	OBB b = RandomOBBContainingPoint(pt, 10.f);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBPlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Plane b = RandomPlaneContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBSphereIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Sphere b = RandomSphereContainingPoint(pt, SCALE);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBCapsuleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Capsule b = RandomCapsuleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBTriangleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Triangle b = RandomTriangleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBFrustumIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Frustum b = RandomFrustumContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBPolyhedronIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Polyhedron b = RandomPolyhedronContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestAABBPolygonIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	AABB a = RandomAABBContainingPoint(pt, 10.f);
	Polygon b = RandomPolygonContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}




void TestOBBOBBIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	OBB b = RandomOBBContainingPoint(pt, 10.f);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBPlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Plane b = RandomPlaneContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBSphereIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Sphere b = RandomSphereContainingPoint(pt, SCALE);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
////	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBCapsuleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Capsule b = RandomCapsuleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBTriangleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Triangle b = RandomTriangleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBFrustumIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Frustum b = RandomFrustumContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBPolyhedronIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Polyhedron b = RandomPolyhedronContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestOBBPolygonIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	OBB a = RandomOBBContainingPoint(pt, 10.f);
	Polygon b = RandomPolygonContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
///	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}





void TestSphereSphereIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Sphere b = RandomSphereContainingPoint(pt, 10.f);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSphereLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSphereRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSphereLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSpherePlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Plane b = RandomPlaneContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSphereCapsuleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Capsule b = RandomCapsuleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSphereTriangleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Triangle b = RandomTriangleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSphereFrustumIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Frustum b = RandomFrustumContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSpherePolyhedronIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Polyhedron b = RandomPolyhedronContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestSpherePolygonIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Sphere a = RandomSphereContainingPoint(pt, 10.f);
	Polygon b = RandomPolygonContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}




void TestFrustumLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestFrustumRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestFrustumLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestFrustumPlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	Plane b = RandomPlaneContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestFrustumCapsuleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	Capsule b = RandomCapsuleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestFrustumTriangleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	Triangle b = RandomTriangleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestFrustumFrustumIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	Frustum b = RandomFrustumContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestFrustumPolyhedronIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	Polyhedron b = RandomPolyhedronContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestFrustumPolygonIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Frustum a = RandomFrustumContainingPoint(pt);
	Polygon b = RandomPolygonContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}




void TestCapsuleLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Capsule a = RandomCapsuleContainingPoint(pt);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestCapsuleRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Capsule a = RandomCapsuleContainingPoint(pt);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestCapsuleLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Capsule a = RandomCapsuleContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestCapsulePlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Capsule a = RandomCapsuleContainingPoint(pt);
	Plane b = RandomPlaneContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestCapsuleCapsuleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Capsule a = RandomCapsuleContainingPoint(pt);
	Capsule b = RandomCapsuleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestCapsuleTriangleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Capsule a = RandomCapsuleContainingPoint(pt);
	Triangle b = RandomTriangleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
///	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestCapsulePolyhedronIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Capsule a = RandomCapsuleContainingPoint(pt);
	Polyhedron b = RandomPolyhedronContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
///	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestCapsulePolygonIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Capsule a = RandomCapsuleContainingPoint(pt);
	Polygon b = RandomPolygonContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}





void TestPolyhedronLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polyhedron a = RandomPolyhedronContainingPoint(pt);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolyhedronRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polyhedron a = RandomPolyhedronContainingPoint(pt);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolyhedronLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polyhedron a = RandomPolyhedronContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
	assert(a.Distance(a.ClosestPoint(b)) < 1e-3f);
//	TODO: The following is problematic due to numerical
//	stability issues at the surface of the Polyhedron.
//	assert(a.Contains(a.ClosestPoint(b)));
	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolyhedronPlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polyhedron a = RandomPolyhedronContainingPoint(pt);
	Plane b = RandomPlaneContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolyhedronTriangleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polyhedron a = RandomPolyhedronContainingPoint(pt);
	Triangle b = RandomTriangleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolyhedronPolyhedronIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polyhedron a = RandomPolyhedronContainingPoint(pt);
	Polyhedron b = RandomPolyhedronContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolyhedronPolygonIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polyhedron a = RandomPolyhedronContainingPoint(pt);
	Polygon b = RandomPolygonContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}



void TestPolygonLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polygon a = RandomPolygonContainingPoint(pt);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolygonRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polygon a = RandomPolygonContainingPoint(pt);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolygonLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polygon a = RandomPolygonContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
	assert(a.Contains(a.ClosestPoint(b)));
	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolygonPlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polygon a = RandomPolygonContainingPoint(pt);
	Plane b = RandomPlaneContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolygonTriangleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polygon a = RandomPolygonContainingPoint(pt);
	Triangle b = RandomTriangleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolygonPolygonIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Polygon a = RandomPolygonContainingPoint(pt);
	Polygon b = RandomPolygonContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
///	assert(b.Contains(b.ClosestPoint(a)));
}



void TestTriangleLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Triangle a = RandomTriangleContainingPoint(pt);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
//	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
	assert(a.Contains(a.ClosestPoint(b)));
	assert(b.Contains(a.ClosestPoint(b)));
	assert(a.Contains(b.ClosestPoint(a)));
	assert(b.Contains(b.ClosestPoint(a)));
}

void TestTriangleRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Triangle a = RandomTriangleContainingPoint(pt);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestTriangleLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Triangle a = RandomTriangleContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
//	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
	assert(a.Contains(a.ClosestPoint(b)));
	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestTrianglePlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Triangle a = RandomTriangleContainingPoint(pt);
	Plane b = RandomPlaneContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestTriangleTriangleIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Triangle a = RandomTriangleContainingPoint(pt);
	Triangle b = RandomTriangleContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
	assert(a.Contains(a.ClosestPoint(b)));
	assert(b.Contains(a.ClosestPoint(b)));
	assert(a.Contains(b.ClosestPoint(a)));
	assert(b.Contains(b.ClosestPoint(a)));
}




void TestPlaneLineIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Plane a = RandomPlaneContainingPoint(pt);
	Line b = RandomLineContainingPoint(pt);
	assert(a.Intersects(b));
///	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPlaneRayIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Plane a = RandomPlaneContainingPoint(pt);
	Ray b = RandomRayContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
	assert(a.Contains(a.ClosestPoint(b)));
	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPlaneLineSegmentIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Plane a = RandomPlaneContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt);
	assert(a.Intersects(b));
	assert(b.Intersects(a));
	assert(a.Distance(b) == 0.f);
	assert(b.Distance(a) == 0.f);
	assert(a.Contains(a.ClosestPoint(b)));
	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPlanePlaneIntersect()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Plane a = RandomPlaneContainingPoint(pt);
	Plane b = RandomPlaneContainingPoint(pt);
	if (a.normal.Equals(b.normal))
		a.d = b.d; // Avoid floating-point imprecision issues in the test: if plane normals are equal, make sure the planes are parallel.
	if (a.normal.Equals(-b.normal))
		a.d = -b.d;
	assert(a.Intersects(b));
	assert(b.Intersects(a));
//	assert(a.Distance(b) == 0.f);
//	assert(b.Distance(a) == 0.f);
//	assert(a.Contains(a.ClosestPoint(b)));
//	assert(b.Contains(a.ClosestPoint(b)));
//	assert(a.Contains(b.ClosestPoint(a)));
//	assert(b.Contains(b.ClosestPoint(a)));
}

void TestPolygonContains2D()
{
	float xmin = 0.f, xmax = 10.f, ymin = 0.f, ymax = 10.f, z = 2.f;

	float3 point = float3((xmax-xmin)/2,(ymax-ymin)/2,z);
	Polygon pol;
	pol.p.push_back(float3(xmin,ymin,z));
	pol.p.push_back(float3(xmax,ymin,z));
	pol.p.push_back(float3(xmax,ymax,z));
	pol.p.push_back(float3(xmin,ymax,z));

	assert(pol.Contains(point));
}

void AddPositiveIntersectionTests()
{
	AddRandomizedTest("AABB-Line positive intersection", TestAABBLineIntersect);
	AddRandomizedTest("AABB-Ray positive intersection", TestAABBRayIntersect);
	AddRandomizedTest("AABB-LineSegment positive intersection", TestAABBLineSegmentIntersect);
	AddRandomizedTest("AABB-AABB positive intersection", TestAABBAABBIntersect);
	AddRandomizedTest("AABB-OBB positive intersection", TestAABBOBBIntersect);
	AddRandomizedTest("AABB-Plane positive intersection", TestAABBPlaneIntersect);
	AddRandomizedTest("AABB-Sphere positive intersection", TestAABBSphereIntersect);
	AddRandomizedTest("AABB-Triangle positive intersection", TestAABBTriangleIntersect);
	AddRandomizedTest("AABB-Capsule positive intersection", TestAABBCapsuleIntersect);
	AddRandomizedTest("AABB-Frustum positive intersection", TestAABBFrustumIntersect);
	AddRandomizedTest("AABB-Polygon positive intersection", TestAABBPolygonIntersect);
	AddRandomizedTest("AABB-Polyhedron positive intersection", TestAABBPolyhedronIntersect);

	AddRandomizedTest("OBB-Line positive intersection", TestOBBLineIntersect);
	AddRandomizedTest("OBB-Ray positive intersection", TestOBBRayIntersect);
	AddRandomizedTest("OBB-LineSegment positive intersection", TestOBBLineSegmentIntersect);
	AddRandomizedTest("OBB-OBB positive intersection", TestOBBOBBIntersect);
	AddRandomizedTest("OBB-Plane positive intersection", TestOBBPlaneIntersect);
	AddRandomizedTest("OBB-Sphere positive intersection", TestOBBSphereIntersect);
	AddRandomizedTest("OBB-Triangle positive intersection", TestOBBTriangleIntersect);
	AddRandomizedTest("OBB-Capsule positive intersection", TestOBBCapsuleIntersect);
	AddRandomizedTest("OBB-Frustum positive intersection", TestOBBFrustumIntersect);
	AddRandomizedTest("OBB-Polygon positive intersection", TestOBBPolygonIntersect);
	AddRandomizedTest("OBB-Polyhedron positive intersection", TestOBBPolyhedronIntersect);

	AddRandomizedTest("Sphere-Line positive intersection", TestSphereLineIntersect);
	AddRandomizedTest("Sphere-Ray positive intersection", TestSphereRayIntersect);
	AddRandomizedTest("Sphere-LineSegment positive intersection", TestSphereLineSegmentIntersect);
	AddRandomizedTest("Sphere-Plane positive intersection", TestSpherePlaneIntersect);
	AddRandomizedTest("Sphere-Sphere positive intersection", TestSphereSphereIntersect);
	AddRandomizedTest("Sphere-Triangle positive intersection", TestSphereTriangleIntersect);
	AddRandomizedTest("Sphere-Capsule positive intersection", TestSphereCapsuleIntersect);
	AddRandomizedTest("Sphere-Frustum positive intersection", TestSphereFrustumIntersect);
	AddRandomizedTest("Sphere-Polygon positive intersection", TestSpherePolygonIntersect);
	AddRandomizedTest("Sphere-Polyhedron positive intersection", TestSpherePolyhedronIntersect);

	AddRandomizedTest("Frustum-Line positive intersection", TestFrustumLineIntersect);
	AddRandomizedTest("Frustum-Ray positive intersection", TestFrustumRayIntersect);
	AddRandomizedTest("Frustum-LineSegment positive intersection", TestFrustumLineSegmentIntersect);
	AddRandomizedTest("Frustum-Plane positive intersection", TestFrustumPlaneIntersect);
	AddRandomizedTest("Frustum-Triangle positive intersection", TestFrustumTriangleIntersect);
	AddRandomizedTest("Frustum-Capsule positive intersection", TestFrustumCapsuleIntersect);
	AddRandomizedTest("Frustum-Frustum positive intersection", TestFrustumFrustumIntersect);
	AddRandomizedTest("Frustum-Polygon positive intersection", TestFrustumPolygonIntersect);
	AddRandomizedTest("Frustum-Polyhedron positive intersection", TestFrustumPolyhedronIntersect);

	AddRandomizedTest("Capsule-Line positive intersection", TestCapsuleLineIntersect);
	AddRandomizedTest("Capsule-Ray positive intersection", TestCapsuleRayIntersect);
	AddRandomizedTest("Capsule-LineSegment positive intersection", TestCapsuleLineSegmentIntersect);
	AddRandomizedTest("Capsule-Plane positive intersection", TestCapsulePlaneIntersect);
	AddRandomizedTest("Capsule-Triangle positive intersection", TestCapsuleTriangleIntersect);
	AddRandomizedTest("Capsule-Capsule positive intersection", TestCapsuleCapsuleIntersect);
	AddRandomizedTest("Capsule-Polygon positive intersection", TestCapsulePolygonIntersect);
	AddRandomizedTest("Capsule-Polyhedron positive intersection", TestCapsulePolyhedronIntersect);

	AddRandomizedTest("Polyhedron-Line positive intersection", TestPolyhedronLineIntersect);
	AddRandomizedTest("Polyhedron-Ray positive intersection", TestPolyhedronRayIntersect);
	AddRandomizedTest("Polyhedron-LineSegment positive intersection", TestPolyhedronLineSegmentIntersect);
	AddRandomizedTest("Polyhedron-Plane positive intersection", TestPolyhedronPlaneIntersect);
	AddRandomizedTest("Polyhedron-Triangle positive intersection", TestPolyhedronTriangleIntersect);
	AddRandomizedTest("Polyhedron-Polygon positive intersection", TestPolyhedronPolygonIntersect);
	AddRandomizedTest("Polyhedron-Polyhedron positive intersection", TestPolyhedronPolyhedronIntersect);

	AddRandomizedTest("Polygon-Line positive intersection", TestPolygonLineIntersect);
	AddRandomizedTest("Polygon-Ray positive intersection", TestPolygonRayIntersect);
	AddRandomizedTest("Polygon-LineSegment positive intersection", TestPolygonLineSegmentIntersect);
	AddRandomizedTest("Polygon-Plane positive intersection", TestPolygonPlaneIntersect);
	AddRandomizedTest("Polygon-Triangle positive intersection", TestPolygonTriangleIntersect);
	AddRandomizedTest("Polygon-Polygon positive intersection", TestPolygonPolygonIntersect);

	AddRandomizedTest("Triangle-Line positive intersection", TestTriangleLineIntersect);
	AddRandomizedTest("Triangle-Ray positive intersection", TestTriangleRayIntersect);
	AddRandomizedTest("Triangle-LineSegment positive intersection", TestTriangleLineSegmentIntersect);
	AddRandomizedTest("Triangle-Plane positive intersection", TestTrianglePlaneIntersect);
	AddRandomizedTest("Triangle-Triangle positive intersection", TestTriangleTriangleIntersect);

	AddRandomizedTest("Plane-Line positive intersection", TestPlaneLineIntersect);
	AddRandomizedTest("Plane-Ray positive intersection", TestPlaneRayIntersect);
	AddRandomizedTest("Plane-LineSegment positive intersection", TestPlaneLineSegmentIntersect);
	AddRandomizedTest("Plane-Plane positive intersection", TestPlanePlaneIntersect);

	AddTest("Polygon::Contains2D", TestPolygonContains2D);
}
