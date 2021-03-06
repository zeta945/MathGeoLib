#include <stdio.h>
#include <stdlib.h>

#include "MathGeoLib.h"
#include "myassert.h"
#include "TestRunner.h"

Line RandomLineContainingPoint(const float3 &pt);
Ray RandomRayContainingPoint(const float3 &pt);
LineSegment RandomLineSegmentContainingPoint(const float3 &pt);

void TestLineLineClosestPoint()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	float3 pt2 = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Line a = RandomLineContainingPoint(pt);
	Line b = RandomLineContainingPoint(pt2);

	float d, d2;
	float3 closestPointA = a.ClosestPoint(b, &d, &d2);
	assert(closestPointA.Equals(a.GetPoint(d), 1e-2f));
	float3 closestPointB = b.GetPoint(d2);
	float D, D2;
	float3 closestPointB2 = b.ClosestPoint(a, &D, &D2);
	assert(EqualAbs(d, D2, 1e-2f));
	assert(EqualAbs(D, d2, 1e-2f));
	assert(closestPointB.Equals(closestPointB2, 1e-2f));
	float3 closestPointA2 = a.GetPoint(D2);
	assert(closestPointA.Equals(closestPointA2, 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, pt.Distance(pt2) + 1e-3f);
	assert(EqualAbs(a.Distance(b), closestPointA.Distance(closestPointB), 1e-2f));
	assert(EqualAbs(b.Distance(a), closestPointA.Distance(closestPointB), 1e-2f));
}

void TestLineRayClosestPoint()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	float3 pt2 = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Line a = RandomLineContainingPoint(pt);
	Ray b = RandomRayContainingPoint(pt2);

	float d, d2;
	float3 closestPointA = a.ClosestPoint(b, &d, &d2);
	assert(closestPointA.Equals(a.GetPoint(d), 1e-2f));
	float3 closestPointB = b.GetPoint(d2);
	float D, D2;
	float3 closestPointB2 = b.ClosestPoint(a, &D, &D2);
	assert(EqualAbs(d, D2));
	assert(EqualAbs(D, d2));
	assert(closestPointB.Equals(closestPointB2, 1e-2f));
	float3 closestPointA2 = a.GetPoint(D2);
	assert(closestPointA.Equals(closestPointA2, 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, pt.Distance(pt2) + 1e-3f);
	assert(EqualAbs(a.Distance(b), closestPointA.Distance(closestPointB), 1e-2f));
	assert(EqualAbs(b.Distance(a), closestPointA.Distance(closestPointB), 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointA.Distance(b.pos) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, a.Distance(b.pos) + 1e-3f);
}

void TestLineLineSegmentClosestPoint()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	float3 pt2 = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Line a = RandomLineContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt2);

	float d, d2;
	float3 closestPointA = a.ClosestPoint(b, &d, &d2);
	assert(closestPointA.Equals(a.GetPoint(d), 1e-2f));
	float3 closestPointB = b.GetPoint(d2);
	float D, D2;
	float3 closestPointB2 = b.ClosestPoint(a, &D, &D2);
	assert(EqualAbs(d, D2));
	assert(EqualAbs(D, d2));
	assert(closestPointB.Equals(closestPointB2, 1e-2f));
	float3 closestPointA2 = a.GetPoint(D2);
	assert(closestPointA.Equals(closestPointA2, 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, pt.Distance(pt2) + 1e-3f);
	assert(EqualAbs(a.Distance(b), closestPointA.Distance(closestPointB), 1e-2f));
	assert(EqualAbs(b.Distance(a), closestPointA.Distance(closestPointB), 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointA.Distance(b.a) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointA.Distance(b.b) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, a.Distance(b.a) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, a.Distance(b.b) + 1e-3f);
}

void TestRayRayClosestPoint()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	float3 pt2 = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Ray a = RandomRayContainingPoint(pt);
	Ray b = RandomRayContainingPoint(pt2);

	float d, d2;
	float3 closestPointA = a.ClosestPoint(b, &d, &d2);
	assert(closestPointA.Equals(a.GetPoint(d), 1e-2f));
	float3 closestPointB = b.GetPoint(d2);
	float D, D2;
	float3 closestPointB2 = b.ClosestPoint(a, &D, &D2);
	assert(EqualAbs(d, D2));
	assert(EqualAbs(D, d2));
	assert(closestPointB.Equals(closestPointB2, 1e-2f));
	float3 closestPointA2 = a.GetPoint(D2);
	assert(closestPointA.Equals(closestPointA2, 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, pt.Distance(pt2) + 1e-3f);
	assert(EqualAbs(a.Distance(b), closestPointA.Distance(closestPointB), 1e-2f));
	assert(EqualAbs(b.Distance(a), closestPointA.Distance(closestPointB), 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointA.Distance(b.pos) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, a.Distance(b.pos) + 1e-3f);

	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointB.Distance(a.pos) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, b.Distance(a.pos) + 1e-3f);
}

void TestRayLineSegmentClosestPoint()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	float3 pt2 = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	Ray a = RandomRayContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt2);

	float d, d2;
	float3 closestPointA = a.ClosestPoint(b, &d, &d2);
	assert(closestPointA.Equals(a.GetPoint(d), 1e-2f));
	float3 closestPointB = b.GetPoint(d2);
	float D, D2;
	float3 closestPointB2 = b.ClosestPoint(a, &D, &D2);
	assert(EqualAbs(d, D2));
	assert(EqualAbs(D, d2));
	assert(closestPointB.Equals(closestPointB2, 1e-2f));
	float3 closestPointA2 = a.GetPoint(D2);
	assert(closestPointA.Equals(closestPointA2, 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, pt.Distance(pt2) + 1e-3f);
	assert(EqualAbs(a.Distance(b), closestPointA.Distance(closestPointB), 1e-2f));
	assert(EqualAbs(b.Distance(a), closestPointA.Distance(closestPointB), 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointA.Distance(b.a) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointA.Distance(b.b) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, a.Distance(b.a) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, a.Distance(b.b) + 1e-3f);

	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointB.Distance(a.pos) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, b.Distance(a.pos) + 1e-3f);
}

void TestLineSegmentLineSegmentClosestPoint()
{
	float3 pt = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	float3 pt2 = float3::RandomBox(rng, -float3(SCALE,SCALE,SCALE), float3(SCALE,SCALE,SCALE));
	LineSegment a = RandomLineSegmentContainingPoint(pt);
	LineSegment b = RandomLineSegmentContainingPoint(pt2);

	float d, d2;
	float3 closestPointA = a.ClosestPoint(b, &d, &d2);
	assert(closestPointA.Equals(a.GetPoint(d), 1e-2f));
	float3 closestPointB = b.GetPoint(d2);
	float D, D2;
	float3 closestPointB2 = b.ClosestPoint(a, &D, &D2);
	assert(EqualAbs(d, D2));
	assert(EqualAbs(D, d2));
	assert(closestPointB.Equals(closestPointB2, 1e-2f));
	float3 closestPointA2 = a.GetPoint(D2);
	assert(closestPointA.Equals(closestPointA2, 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, pt.Distance(pt2) + 1e-3f);
	assert(EqualAbs(a.Distance(b), closestPointA.Distance(closestPointB), 1e-2f));
	assert(EqualAbs(b.Distance(a), closestPointA.Distance(closestPointB), 1e-2f));

	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointA.Distance(b.a) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointA.Distance(b.b) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, a.Distance(b.a) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, a.Distance(b.b) + 1e-3f);

	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointB.Distance(a.a) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, closestPointB.Distance(a.b) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, b.Distance(a.a) + 1e-3f);
	assertcmp(closestPointA.Distance(closestPointB), <=, b.Distance(a.b) + 1e-3f);
}
void AddLineTests()
{
	AddRandomizedTest("Line-Line ClosestPoint", TestLineLineClosestPoint);
	AddRandomizedTest("Line-Ray ClosestPoint", TestLineRayClosestPoint);
	AddRandomizedTest("Line-LineSegment ClosestPoint", TestLineLineSegmentClosestPoint);

	AddRandomizedTest("Ray-Ray ClosestPoint", TestRayRayClosestPoint);
	AddRandomizedTest("Ray-LineSegment ClosestPoint", TestRayLineSegmentClosestPoint);

	AddRandomizedTest("LineSegment-LineSegment ClosestPoint", TestLineSegmentLineSegmentClosestPoint);
}
