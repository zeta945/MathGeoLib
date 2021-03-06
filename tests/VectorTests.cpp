#include <stdio.h>
#include <stdlib.h>

#include "MathGeoLib.h"
#include "myassert.h"
#include "TestRunner.h"

void TestFloat4Swizzled()
{
	float4 f(float2(1,2),3,4);
	float4 f2 = f.Swizzled(2,0,1,3);
	float f3[4] = { 3, 1, 2, 4 };
	assert(f2.Equals(float4(f3)));
}

void TestFloat4LengthSq3()
{
	float4 f(-1.f,2.f,3.f,1000000.f);
	assert(EqualAbs(f.LengthSq3(), 14.f));
}

void TestFloat4Length3()
{
	float4 f(-1.f,2.f,3.f,1000000.f);
	assert(EqualAbs(f.Length3(), Sqrt(14.f)));
}

void TestFloat4LengthSq4()
{
	float4 f(-1.f,2.f,3.f,-4.f);
	assert(EqualAbs(f.LengthSq4(), 30.f));
}

void TestFloat4Length4()
{
	float4 f(-1.f,2.f,3.f,-4.f);
	assert(EqualAbs(f.Length4(), Sqrt(30.f)));
}

void TestFloat4Normalize3()
{
	float4 f(-1.f, 2.f, 3.f, 1000.f);
	float oldLength = f.Normalize3();
	assert(oldLength > 0.f);
	assert(EqualAbs(f.x, -1.f / Sqrt(14.f)));
	assert(EqualAbs(f.y, 2.f / Sqrt(14.f)));
	assert(EqualAbs(f.z, 3.f / Sqrt(14.f)));
	assert(EqualAbs(f.w, 1000.f));

	float4 f2(0,0,0, 1000.f);
	oldLength = f2.Normalize3();
	assert(oldLength == 0.f);
	assert(f2.x == 1.f);
	assert(f2.y == 0.f);
	assert(f2.z == 0.f);
	assert(f2.w == 1000.f);
}

void TestFloat4Normalized3()
{
	float4 f(-1.f, 2.f, 3.f, 1000.f);
	float4 f2 = f.Normalized3();
	assert(EqualAbs(f2.x, -1.f / Sqrt(14.f)));
	assert(EqualAbs(f2.y, 2.f / Sqrt(14.f)));
	assert(EqualAbs(f2.z, 3.f / Sqrt(14.f)));
	assert(EqualAbs(f2.w, 1000.f));
}

void TestFloat4Normalize4()
{
	float4 f(-1.f, 2.f, 3.f, 4.f);
	float oldLength = f.Normalize4();
	assert(oldLength > 0);
	assert(EqualAbs(f.x, -1.f / Sqrt(30.f)));
	assert(EqualAbs(f.y, 2.f / Sqrt(30.f)));
	assert(EqualAbs(f.z, 3.f / Sqrt(30.f)));
	assert(EqualAbs(f.w, 4.f / Sqrt(30.f)));

	float4 f2(0,0,0, 0.f);
	oldLength = f2.Normalize4();
	assert(oldLength == 0.f);
	assert(f2.x == 1.f);
	assert(f2.y == 0.f);
	assert(f2.z == 0.f);
	assert(f2.w == 0.f);
}

void TestFloat4Normalized4()
{
	float4 f(-1.f, 2.f, 3.f, -4.f);
	float4 f2 = f.Normalized4();
	assert(EqualAbs(f2.x, -1.f / Sqrt(30.f)));
	assert(EqualAbs(f2.y, 2.f / Sqrt(30.f)));
	assert(EqualAbs(f2.z, 3.f / Sqrt(30.f)));
	assert(EqualAbs(f2.w, -4.f / Sqrt(30.f)));
}

void TestFloat4NormalizeW()
{
	float4 f(-2.f, -4.f, 8.f, 2.f);
	f.NormalizeW();
	assert(f.Equals(float4(-1.f, -2.f, 4.f, 1.f)));
}

void TestFloat4Scale3()
{
	float4 f(-2.f, -4.f, 8.f, 1000.f);
	f.Scale3(100.f);
	assert(f.Equals(float4(-200.f, -400.f, 800.f, 1000.f)));
}

void TestFloat4ScaleToLength3()
{
	float4 f(-1.f, 2.f, 3.f, 1000.f);
	float4 f2 = f.ScaledToLength3(10.f);
	float oldLength = f.ScaleToLength3(10.f);
	assert(f.Equals(f2));
	assert(EqualAbs(oldLength, Sqrt(14.f)));
	assert(EqualAbs(f.x, -1.f * 10.f / oldLength));
	assert(EqualAbs(f.y, 2.f * 10.f / oldLength));
	assert(EqualAbs(f.z, 3.f * 10.f / oldLength));
	assert(EqualAbs(f.w, 1000.f));
}

void TestFloat4SumOfElements()
{
	float4 f(-1.f, 4.f, 20.f, -100.f);
	assert(EqualAbs(f.SumOfElements(), -77.f));
}

void TestFloat4ProductOfElements()
{
	float4 f(-1.f, 2.f, 4.f, -8.f);
	assert(EqualAbs(f.ProductOfElements(), 64.f));
}

void TestFloat4AverageOfElements()
{
	float4 f(-2.f, 2.f, 4.f, -8.f);
	assert(EqualAbs(f.AverageOfElements(), -1.f));
}

void TestFloat4Abs()
{
	float4 f(-2.f, 2.f, 4.f, -8.f);
	assert(f.Abs().Equals(float4(2.f, 2.f, 4.f, 8.f)));
}

void TestFloat4Neg3()
{
	float4 f(-2.f, 2.f, 4.f, -8.f);
	assert(f.Neg3().Equals(float4(2.f, -2.f, -4.f, -8.f)));
}

void TestFloat4Neg4()
{
	float4 f(-2.f, 2.f, 4.f, -8.f);
	assert(f.Neg4().Equals(float4(2.f, -2.f, -4.f, 8.f)));
}

void TestFloat4Recip3()
{
	float4 f(-1.f, 2.f, 4.f, -8.f);
	assert(f.Recip3().Equals(float4(-1.f, 0.5f, 0.25f, -8.f)));
}

void TestFloat4Recip4()
{
	float4 f(-1.f, 2.f, 4.f, -8.f);
	assert(f.Recip4().Equals(float4(-1.f, 0.5f, 0.25f, -0.125f)));
}

void TestFloat4RecipFast4()
{
	float4 f(-1.f, 2.f, 4.f, -8.f);
	assert(f.RecipFast4().Equals(float4(-1.f, 0.5f, 0.25f, -0.125f)));
}

void TestFloat4Min()
{
	float4 f(-1.f, 2.f, 4.f, -8.f);
	assert(f.Min(-4.f).Equals(float4(-4.f, -4.f, -4.f, -8.f)));
	assert(f.Min(float4(-3.f, 20.f, -2.f, 9.f)).Equals(-3.f, 2.f, -2.f, -8.f));
}

void TestFloat4Max()
{
	float4 f(-1.f, 2.f, 4.f, -8.f);
	assert(f.Max(-4.f).Equals(float4(-1.f, 2.f, 4.f, -4.f)));
	assert(f.Max(float4(-3.f, 20.f, -2.f, 9.f)).Equals(-1.f, 20.f, 4.f, 9.f));
}

void TestFloat4Clamp()
{
	float4 f(-1.f, 2.f, 4.f, -8.f);
	assert(f.Clamp(-2.f, 2.f).Equals(float4(-1.f, 2.f, 2.f, -2.f)));
	assert(f.Clamp(float4(0.f, -1.f, -10.f, -8.f), float4(10.f, 0.f, 10.f, -8.f)).Equals(float4(0.f, 0.f, 4.f, -8.f)));
}

void TestFloat4Clamp01()
{
	float4 f(-1.f, 0.f, 0.5f, 2.f);
	assert(f.Clamp01().Equals(float4(0.f, 0.f, 0.5f, 1.f)));
}

void TestFloat4DistanceSq3()
{
	float4 f(1.f, 2.f, 3.f, 4.f);
	float4 f2(-1.f, -2.f, -3.f, -4.f);
	assert(EqualAbs(f.Distance3Sq(f2), 56.f));
}

void TestFloat4Distance3()
{
	float4 f(1.f, 2.f, 3.f, 4.f);
	float4 f2(-1.f, -2.f, -3.f, -4.f);
	assert(EqualAbs(f.Distance3(f2), Sqrt(56.f)));
}

void TestFloat4DistanceSq4()
{
	float4 f(1.f, 2.f, 3.f, 4.f);
	float4 f2(-1.f, -2.f, -3.f, -4.f);
	assert(EqualAbs(f.Distance4Sq(f2), 120.f));
}

void TestFloat4Distance4()
{
	float4 f(1.f, 2.f, 3.f, 4.f);
	float4 f2(-1.f, -2.f, -3.f, -4.f);
	assert(EqualAbs(f.Distance4(f2), Sqrt(120.f)));
}

void TestFloat4Dot3()
{
	float4 f(-1.f, 2.f, 3.f, -4.f);
	float3 f2(2.f, -1.f, -3.f);
	float4 f3(2.f, -1.f, 0.f, 4.f);
	assert(EqualAbs(f.Dot3(f2), -13.f));
	assert(EqualAbs(f.Dot3(f3), -4.f));
}

void TestFloat4Dot4()
{
	float4 f(-1.f, 2.f, 3.f, -4.f);
	float4 f2(2.f, -1.f, 0.f, 4.f);
	assert(EqualAbs(f.Dot4(f2), -20.f));
}

void TestFloat4Cross3()
{
	float4 f(-1.f, 2.f, 3.f, -4.f);
	float4 f2(2.f, -1.f, 0.f, 4.f);
	assert(f.Cross3(f2).Equals(f.Cross3(f2.xyz())));

	float4 f3 = f.Cross3(f2);
	assert(f3.Equals(float4(3.f, 6.f, -3.f, 0.f)));

	float4 z = float4::unitX.Cross3(float4::unitY);
	float4 y = float4::unitZ.Cross3(float4::unitX);
	float4 x = float4::unitY.Cross3(float4::unitZ);
	assert(x.Equals(float4(1,0,0,0)));
	assert(y.Equals(float4(0,1,0,0)));
	assert(z.Equals(float4(0,0,1,0)));
}

void TestFloat4FromScalar()
{
	float4 f = float4::FromScalar(1.f);
	assert(f.Equals(float4(1,1,1,1)));
	f.SetFromScalar(-1.f);
	assert(f.Equals(float4(-1,-1,-1,-1)));

	float4 f2(3.f, 3.f, 3.f, -9.f);
	f.SetFromScalar(3.f, -9.f);
	assert(f.Equals(f2));
}

void TestFloat4Set()
{
	float4 f = float4(1,2,3,4);
	f.Set(5,6,7,8);
	assert(f.Equals(float4(5,6,7,8)));
}

void TestFloat4OpAdd()
{
	float4 f = float4(1,2,3,4);
	float4 f2 = float4(-5.f, -6.f, -7.f, -8.f);
	float4 f3 = f + f2;
	assert(f3.Equals(float4(-4.f, -4.f, -4.f, -4.f)));
}

void TestFloat2OpAddUnary()
{
	float2 f(1,2);
	float2 g = +f;
	assert(f.Equals(g));
}

void TestFloat3OpAddUnary()
{
	float3 f(1,2,3);
	float3 g = +f;
	assert(f.Equals(g));
}

void TestFloat4OpAddUnary()
{
	float4 f(1,2,3,4);
	float4 g = +f;
	assert(f.Equals(g));
}

void TestFloat4OpSub()
{
	float4 f = float4(1,2,3,4);
	float4 f2 = float4(-5.f, -6.f, -7.f, -8.f);
	float4 f3 = f - f2;
	assert(f3.Equals(float4(6.f, 8.f, 10.f, 12.f)));
}

void TestFloat4OpNeg()
{
	float4 f = float4(1,2,3,4);
	float4 f3 = -f;
	assert(f3.Equals(float4(-1.f, -2.f, -3.f, -4.f)));
}

void TestFloat4OpMul()
{
	float4 f = float4(1,2,3,4);
	float scalar = -2.f;
	
	float4 f2 = f * scalar;
	float4 f3 = scalar * f;
	assert(f2.Equals(f3));

	assert(f2.Equals(float4(-2.f, -4.f, -6.f, -8.f)));
}

void TestFloat4OpDiv()
{
	float4 f = float4(1,2,4,8);
	float scalar = -2.f;
	
	float4 f2 = f / scalar;
	assert(f2.Equals(float4(-0.5f, -1.f, -2.f, -4.f)));
}

void TestFloat4OpAddAssign()
{
	float4 f = float4(1,2,3,4);
	float4 f2 = float4(-5.f, -6.f, -7.f, -8.f);
	float4 f3 = f;
	f3 += f2;
	assert(f3.Equals(float4(-4.f, -4.f, -4.f, -4.f)));
}

void TestFloat4OpSubAssign()
{
	float4 f = float4(1,2,3,4);
	float4 f2 = float4(-5.f, -6.f, -7.f, -8.f);
	float4 f3 = f;
	f3 -= f2;
	assert(f3.Equals(float4(6.f, 8.f, 10.f, 12.f)));
}

void TestFloat4OpMulAssign()
{
	float4 f = float4(1,2,3,4);
	float scalar = -2.f;
	
	float4 f2 = f;
	f2 *= scalar;
	assert(f2.Equals(float4(-2.f, -4.f, -6.f, -8.f)));
}

void TestFloat4OpDivAssign()
{
	float4 f = float4(1,2,4,8);
	float scalar = -2.f;
	
	float4 f2 = f;
	f2 /= scalar;
	assert(f2.Equals(float4(-0.5f, -1.f, -2.f, -4.f)));
}

void TestFloat4Add()
{
	float4 f = float4(1,2,3,4);
	float scalar = 5.f;
	float4 f2 = f.Add(scalar);
	assert(f2.Equals(float4(6.f, 7.f, 8.f, 9.f)));
}

void TestFloat4Sub()
{
	float4 f = float4(1,2,3,4);
	float scalar = 5.f;
	float4 f2 = f.Sub(scalar);
	assert(f2.Equals(float4(-4.f, -3.f, -2.f, -1.f)));
}

void TestFloat4SubLeft()
{
	float4 f = float4(1,2,3,4);
	float scalar = 5.f;
	float4 f2 = f.SubLeft(scalar);
	assert(f2.Equals(float4(4.f, 3.f, 2.f, 1.f)));
}

void TestFloat4DivLeft()
{
	float4 f = float4(-1,2,4,8);
	float scalar = -8.f;
	float4 f2 = f.DivLeft(scalar);
	assert(f2.Equals(float4(8.f, -4.f, -2.f, -1.f)));
}

void TestFloat4Mul()
{
	float4 f = float4(1,2,3,4);
	float4 f2 = float4(-2.f, -4.f, 2.f, 1.f);
	assert(f.Mul(f2).Equals(float4(-2.f, -8.f, 6.f, 4.f)));
}

void TestFloat4Div()
{
	float4 f = float4(1,1,4,8);
	float4 f2 = float4(-2.f, -4.f, 2.f, 1.f);
	assert(f.Div(f2).Equals(float4(-0.5f, -0.25f, 2.f, 8.f)));
}

void AddVectorTests()
{
	AddTest("float4::Swizzled", TestFloat4Swizzled);
	AddTest("float4::LengthSq3", TestFloat4LengthSq3);
	AddTest("float4::Length3", TestFloat4Length3);
	AddTest("float4::LengthSq4", TestFloat4LengthSq4);
	AddTest("float4::Length4", TestFloat4Length4);
	AddTest("float4::Normalize3", TestFloat4Normalize3);
	AddTest("float4::Normalized3", TestFloat4Normalized3);
	AddTest("float4::Normalize4", TestFloat4Normalize4);
	AddTest("float4::Normalized4", TestFloat4Normalized4);
	AddTest("float4::NormalizeW", TestFloat4NormalizeW);
	AddTest("float4::Scale3", TestFloat4Scale3);
	AddTest("float4::ScaleToLength3", TestFloat4ScaleToLength3);
	AddTest("float4::SumOfElements", TestFloat4SumOfElements);
	AddTest("float4::ProductOfElements", TestFloat4ProductOfElements);
	AddTest("float4::AverageOfElements", TestFloat4AverageOfElements);
	AddTest("float4::Abs", TestFloat4Abs);
	AddTest("float4::Neg3", TestFloat4Neg3);
	AddTest("float4::Neg4", TestFloat4Neg4);
	AddTest("float4::Recip3", TestFloat4Recip3);
	AddTest("float4::Recip4", TestFloat4Recip4);
	AddTest("float4::RecipFast4", TestFloat4RecipFast4);
	AddTest("float4::Min", TestFloat4Min);
	AddTest("float4::Max", TestFloat4Max);
	AddTest("float4::Clamp", TestFloat4Clamp);
	AddTest("float4::Clamp01", TestFloat4Clamp01);
	AddTest("float4::DistanceSq3", TestFloat4DistanceSq3);
	AddTest("float4::Distance3", TestFloat4Distance3);
	AddTest("float4::DistanceSq4", TestFloat4DistanceSq4);
	AddTest("float4::Distance4", TestFloat4Distance4);
	AddTest("float4::Dot3", TestFloat4Dot3);
	AddTest("float4::Dot4", TestFloat4Dot4);
	AddTest("float4::Cross3", TestFloat4Cross3);
	AddTest("float4::SetFromScalar", TestFloat4FromScalar);
	AddTest("float4::Set", TestFloat4Set);
	AddTest("float4::operator+", TestFloat4OpAdd);
	AddTest("float2::operator+()", TestFloat2OpAddUnary);
	AddTest("float3::operator+()", TestFloat3OpAddUnary);
	AddTest("float4::operator+()", TestFloat4OpAddUnary);
	AddTest("float4::operator-", TestFloat4OpSub);
	AddTest("float4::operator-()", TestFloat4OpNeg);
	AddTest("float4::operator*", TestFloat4OpMul);
	AddTest("float4::operator/", TestFloat4OpDiv);
	AddTest("float4::operator+=", TestFloat4OpAddAssign);
	AddTest("float4::operator-=", TestFloat4OpSubAssign);
	AddTest("float4::operator*=", TestFloat4OpMulAssign);
	AddTest("float4::operator/=", TestFloat4OpDivAssign);
	AddTest("float4::Add", TestFloat4Add);
	AddTest("float4::Sub", TestFloat4Sub);
	AddTest("float4::SubLeft", TestFloat4SubLeft);
	AddTest("float4::DivLeft", TestFloat4DivLeft);
	AddTest("float4::Mul", TestFloat4Mul);
	AddTest("float4::Div", TestFloat4Div);
}
