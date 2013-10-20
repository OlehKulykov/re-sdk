/*
 * Copyright (c) 2002-2007 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * REPRODUCTION IN WHOLE OR IN PART IS PROHIBITED WITHOUT THE WRITTEN
 * CONSENT OF THE COPYRIGHT OWNER.
 *
 */

/* $ Revision 1.1.2 $ */


//#include "StdAfx.H"
#include "Geometry.h"

Engine::Geometry::CPoint Engine::Geometry::CPoint::FromVector2(const CVector2 &v)
{
	return CPoint((int) floor(v.x + 0.5f), (int) floor(v.y + 0.5f));
}

Engine::CArchive &Engine::Geometry::operator<<(Engine::CArchive &ar, Engine::Geometry::CPoint p)
{
	ar << p.x;
	ar << p.y;
	
	return ar;
}

Engine::CArchive &Engine::Geometry::operator>>(Engine::CArchive &ar, Engine::Geometry::CPoint &p)
{
	ar >> p.x;
	ar >> p.y;
	
	return ar;
}

Engine::CArchive &Engine::Geometry::operator<<(Engine::CArchive &ar, Engine::Geometry::CVector2 v)
{
	ar << v.x;
	ar << v.y;
	
	return ar;
}

Engine::CArchive &Engine::Geometry::operator>>(Engine::CArchive &ar, Engine::Geometry::CVector2 &v)
{
	ar >> v.x;
	ar >> v.y;
	
	return ar;
}

Engine::CArchive &Engine::Geometry::operator<<(Engine::CArchive &ar, Engine::Geometry::CRect r)
{
	ar << r.Left;
	ar << r.Top;
	ar << r.Right;
	ar << r.Bottom;
	
	return ar;
}

Engine::CArchive &Engine::Geometry::operator>>(Engine::CArchive &ar, Engine::Geometry::CRect &r)
{
	ar >> r.Left;
	ar >> r.Top;
	ar >> r.Right;
	ar >> r.Bottom;
	
	return ar;
}

Engine::Geometry::CPoint::operator Engine::Geometry::CVector2() const
{
	return CVector2((float) x, (float) y);
}

int Engine::Geometry::Noise::NoisePermTable[0x402];
float Engine::Geometry::Noise::NoiseTable[0x402];

void Engine::Geometry::Noise::Init()
{
	static bool bInit = false;
	
	if (bInit)
		return;
	
	bInit = true;
	
	int i;
	
	srand(0x85e85);
	
	for (i = 0; i < 0x200; i ++)
	{
		NoisePermTable[i] = i;
		
		float v = (float) ((rand() % 0x400) - 0x200) / 0x200;
		NoiseTable[i] = v;
	}
	
	while (--i)
	{
		int v = NoisePermTable[i];
		int j = rand() % 0x200;
		
		NoisePermTable[i] = NoisePermTable[j];
		NoisePermTable[j] = v;
	}
	
	{
		for (i = 0; i < 0x202; i ++)
		{
			NoiseTable[i + 0x200] = NoiseTable[i];
			NoisePermTable[i + 0x200] = NoisePermTable[i];
		}
	}
}

#define NoiseSetup(i, b0, b1, r0, r1)   \
t = pnt##i + 10000.0f;                          \
ft = (float) floor(t);                          \
b0 = ((int) ft) & 0x1ff;                        \
b1 = (b0 + 1) & 0x1ff;                          \
r0 = t - ft;                                            \
r1 = r0 - 1.0f;

#define NoiseSetup2(i, b0, b1, r0, r1)  \
t = pnt##i + 10000.0f;                          \
ft = (float) floor(t);                          \
b0 = ((int) ft);                                        \
b1 = (b0 + 1);                                          \
r0 = t - ft;                                            \
r1 = r0 - 1.0f;

#define NoiseLerp(t, a,  b)    ((a) + ((b) - (a)) * (t))
#define NoiseCurve(t)          ((t) * (t) * (3.0f - 2.0f * (t)))
#define NoiseIntCurve(t)       (-0.5f * (t) * (t) * (t) * ((t) - 2.0f))

/* Maple 4ever */
float Engine::Geometry::Noise::Noise11(float pnt0)
{
	int bx0, bx1;
	float rx0, rx1;
	float sx;
	float t;
	float u, v;
	float ft;
	
	Init();
	
	NoiseSetup(0, bx0, bx1, rx0, rx1);
	
	sx = NoiseCurve(rx0);
	
	u = rx0 * NoiseTable[NoisePermTable[bx0]];
	v = rx1 * NoiseTable[NoisePermTable[bx1]];
	
	return NoiseLerp(sx, u, v);
}

float NoiseIntFuncEnd(float v0, float v1, float e)
{
	float t1 = e*e;
    float t2 = t1*e;
	float t17 = -t1*(8.0*t2*v1-8.0*t2*v0-25.0*t1*v1+15.0*v0*t1+20.0*e*v1-10.0*v0)/
	20.0;
	
	return t17;
}

float NoiseIntFuncStart(float v0, float v1, float s)
{
	float t2 = (s-1.0)*(s-1.0);
	float t3 = s*s;
	float t4 = t3*s;
	float t20 = t2*(8.0*v1*t4-8.0*v0*t4-v0*t3-9.0*t3*v1-6.0*s*v1+6.0*s*v0-3.0*v1+
					3.0*v0)/20.0;
	
	return t20;
}

float NoiseIntFunc(float v0, float v1, float s, float e)
{
	float t2 = e*e;
	float t3 = t2*t2;
	float t8 = t2*e;
	float t13 = t8*s;
	float t20 = t2*s;
	float t25 = s*s;
	float t32 = 8.0f*t3*v1-8.0f*v0*t3-25.0f*v1*t8+15.0f*t8*v0+8.0f*t13*v1-8.0f*t13*v0+
	20.0f*t2*v1-25.0f*t20*v1+15.0f*t20*v0+8.0f*t2*t25*v1-8.0f*t2*v0*t25;
	float t33 = e*v0;
	float t44 = t25*s;
	float t57 = t25*t25;
	float t62 = -10.0f*t33+20.0f*e*s*v1-25.0f*e*t25*v1+15.0f*t33*t25+8.0f*e*v1*t44-8.0f*
	t33*t44-10.0f*s*v0+20.0f*t25*v1-25.0f*v1*t44+15.0f*v0*t44+8.0f*v1*t57-8.0f*v0*t57;
	float t66 = -(e-s)*(t32+t62)/20.0f;
	
	return t66;
}

float Engine::Geometry::Noise::Noise11Int(float pnt0s, float pnt0e)
{
	int bx0s, bx1s;
	float rx0s, rx1s;
	int bx0e, bx1e;
	float rx0e, rx1e;
	float t;
	float ft;
	//float u, v;
	
	if (pnt0e < pnt0s)
	{
		t = pnt0e;
		pnt0e = pnt0s;
		pnt0s = t;
	}
	
	Init();
	
	NoiseSetup2(0s, bx0s, bx1s, rx0s, rx1s);
	NoiseSetup2(0e, bx0e, bx1e, rx0e, rx1e);
	
	float sx = 0.0f;
	for (int ix = bx1s; ix < bx0e; ix ++)
	{
		float sv = NoiseTable[NoisePermTable[ix & 0x1ff]];
		float ev = NoiseTable[NoisePermTable[(ix + 1) & 0x1ff]];
		sx += (sv - ev) * 0.15f;
	}
	
	if (bx0s != bx0e)
	{
		{
			float v0 = NoiseTable[NoisePermTable[bx0s & 0x1ff]];
			float v1 = NoiseTable[NoisePermTable[bx1s & 0x1ff]];
			
			sx += NoiseIntFuncStart(v0, v1, rx0s);
		}
		
		{
			float v0 = NoiseTable[NoisePermTable[bx0e & 0x1ff]];
			float v1 = NoiseTable[NoisePermTable[bx1e & 0x1ff]];
			
			sx += NoiseIntFuncEnd(v0, v1, rx0e);
		}
	}
	else
	{
		float v0 = NoiseTable[NoisePermTable[bx0s & 0x1ff]];
		float v1 = NoiseTable[NoisePermTable[bx1s & 0x1ff]];
		
		sx += NoiseIntFunc(v0, v1, rx0s, rx0e);
	}
	
	return sx;
}

#undef NoiseSetup
#undef NoiseLerp
#undef NoiseCurve

void Engine::Geometry::CRectF::Set()
{
	Left = Top = Right = Bottom = 0.0f;
}

void Engine::Geometry::CRectF::Set(const CVector2 &TopLeft, const CVector2 &BottomRight)
{
	Left     = TopLeft.x;
	Top      = TopLeft.y;
	Right    = BottomRight.x;
	Bottom   = BottomRight.y;
}

void Engine::Geometry::CRectF::Set(float _Left, float _Top, float _Right, float _Bottom)
{
	Left   = _Left;
	Top    = _Top;
	Right  = _Right;
	Bottom = _Bottom;
}

void Engine::Geometry::CRectF::Set(const CVector2 &TopLeft, float Width, float Height, bool bCenter)
{
	if (!bCenter)
	{
		Left   = TopLeft.x;
		Top    = TopLeft.y;
		Right  = TopLeft.x + Width;
		Bottom = TopLeft.y + Height;
	}
	else
	{
		Width  *= 0.5f;
		Height *= 0.5f;
		
		Left = TopLeft.x - Width;
		Top  = TopLeft.y - Height;
		
		Right  = TopLeft.x + Width;
		Bottom = TopLeft.y + Height;
	}
}

void Engine::Geometry::CRectF::Set(const CRectF &r)
{
	Left   = r.Left;
	Top    = r.Top;
	Right  = r.Right;
	Bottom = r.Bottom;
}

void Engine::Geometry::CRectF::Inflate(const CRectF &Offset)
{
	Left -= Offset.Left;
	Top -= Offset.Top;
	
	Right += Offset.Right;
	Bottom += Offset.Bottom;
	
	Validate();
}

Engine::Geometry::CRectF Engine::Geometry::CRectF::operator+( const CVector2 &v ) const
{
	return CRectF(Left + v.x, Top + v.y, Right + v.x, Bottom + v.y);
}

Engine::Geometry::CRectF Engine::Geometry::CRectF::operator-( const CVector2 &v ) const
{
	return CRectF(Left - v.x, Top - v.y, Right - v.x, Bottom - v.y);
}

Engine::Geometry::CRectF& Engine::Geometry::CRectF::operator-=( const CVector2 &v )
{
	Left   -= v.x;
	Top    -= v.y;
	Right  -= v.x;
	Bottom -= v.y;
	
	return *this;
}

Engine::Geometry::CRectF& Engine::Geometry::CRectF::operator+=( const CVector2 &v )
{
	Left   += v.x;
	Top    += v.y;
	Right  += v.x;
	Bottom += v.y;
	
	return *this;
}

bool Engine::Geometry::CRectF::IsValid() const
{
    return Left <= Right &&
	Top <= Bottom;
}

void Engine::Geometry::CRectF::Validate()
{
    if (Left > Right)
        Left = Right;
	
    if (Top > Bottom)
        Top = Bottom;
}

void Engine::Geometry::CRectF::Offset(const CVector2 &v)
{
    Left += v.x;
    Top += v.y;
	
    Right += v.x;
    Bottom += v.y;
}

void Engine::Geometry::CRectF::Offset(float DeltaX, float DeltaY)
{
    Left += DeltaX;
    Top += DeltaY;
	
    Right += DeltaX;
    Bottom += DeltaY;
}

void Engine::Geometry::CRectF::Inflate(float DeltaX, float DeltaY)
{
    Left -= DeltaX;
    Top  -= DeltaY;
	
    Right  += DeltaX;
    Bottom += DeltaY;
	
    Validate();
}

void Engine::Geometry::CRectF::Deflate(float DeltaX, float DeltaY)
{
    Left += DeltaX;
    Top  += DeltaY;
	
    Right  -= DeltaX;
    Bottom -= DeltaY;
	
    Validate();
}

void Engine::Geometry::CRectF::GetCenter(float &x, float &y) const
{
    x = (Left + Right ) * 0.5f;
    y = (Top  + Bottom) * 0.5f;
}

void Engine::Geometry::CRectF::GetCenter(CVector2 &v) const
{
    v.x = (Left + Right ) * 0.5f;
    v.y = (Top  + Bottom) * 0.5f;
}

Engine::Geometry::CVector2 Engine::Geometry::CRectF::GetCenter() const
{
    return CVector2((Left + Right) * 0.5f,
                    (Top + Bottom) * 0.5f);
}

void Engine::Geometry::CRectF::Scale(float ScaleX, float ScaleY)
{
    float x, y;
    GetCenter(x, y);
	
    float WidthScaled  = (Right - Left) * 0.5f * ScaleX;
    float HeightScaled = (Bottom - Top) * 0.5f * ScaleY;
	
    Left = x - WidthScaled;
    Top  = y - HeightScaled;
	
    Right  = x + WidthScaled;
    Bottom = y + HeightScaled;
}

bool Engine::Geometry::CRect::IsIntersect(const CRect &r) const
{
    int dx = (Left + Right) - (r.Left + r.Right);
    int dy = (Top + Bottom) - (r.Top + r.Bottom);
	
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	
    return dx <= GetWidth() + r.GetWidth() &&
	dy <= GetHeight() + r.GetHeight();
}

bool Engine::Geometry::CRectF::IsIntersect(const CRectF &r) const
{
    float dx = (Left + Right) -
	(r.Left + r.Right);
	
    float dy = (Top + Bottom) -
	(r.Top + r.Bottom);
	
    return fabs(dx) <= GetWidth() + r.GetWidth() &&
	fabs(dy) <= GetHeight() + r.GetHeight();
}

bool Engine::Geometry::CRectF::Intersect(const CRectF &r, CRectF &Result) const
{
    if (IsIntersect(r))
    {
		Result.Left   = Left   > r.Left   ? Left   : r.Left;
		Result.Top    = Top    > r.Top    ? Top    : r.Top;
		Result.Right  = Right  < r.Right  ? Right  : r.Right;
		Result.Bottom = Bottom < r.Bottom ? Bottom : r.Bottom;
		
        return true;
    }
	
    return false;
}

void Engine::Geometry::CRectF::Union(const CRectF &r, CRectF &Result) const
{
	Result.Left   = Left   < r.Left   ? Left   : r.Left;
	Result.Top    = Top    < r.Top    ? Top    : r.Top;
	Result.Right  = Right  > r.Right  ? Right  : r.Right;
	Result.Bottom = Bottom > r.Bottom ? Bottom : r.Bottom;
}

/*
 Engine::Geometry::CRectF Engine::Geometry::CRectF::operator *(const CMatrix22 &m) const
 {
 CVector2 TopLeft(m * TopLeft);
 CVector2 TopRight(m * CVector2(BottomRight.x, TopLeft.y));
 CVector2 BottomRight(m * BottomRight);
 CVector2 BottomLeft(m * CVector2(TopLeft.x, BottomRight.y));
 
 CVector2 ResultTopLeft(TopLeft);
 
 ResultTopLeft.Min(TopRight);
 ResultTopLeft.Min(BottomRight);
 ResultTopLeft.Min(BottomLeft);
 
 CVector2 ResultBottomRight(BottomRight);
 
 ResultBottomRight.Max(TopLeft);
 ResultBottomRight.Max(TopRight);
 ResultBottomRight.Max(BottomLeft);
 
 return CRectF(ResultTopLeft, ResultBottomRight);
 }
 
 Engine::Geometry::CRectF Engine::Geometry::CRectF::operator *(const CMatrix23 &m) const
 {
 CVector2 TopLeft(m * TopLeft);
 CVector2 TopRight(m * CVector2(BottomRight.x, TopLeft.y));
 CVector2 BottomRight(m * BottomRight);
 CVector2 BottomLeft(m * CVector2(TopLeft.x, BottomRight.y));
 
 CVector2 ResultTopLeft(TopLeft);
 
 ResultTopLeft.Min(TopRight);
 ResultTopLeft.Min(BottomRight);
 ResultTopLeft.Min(BottomLeft);
 
 CVector2 ResultBottomRight(BottomRight);
 
 ResultBottomRight.Max(TopLeft);
 ResultBottomRight.Max(TopRight);
 ResultBottomRight.Max(BottomLeft);
 
 return CRectF(ResultTopLeft, ResultBottomRight);
 }
 
 Engine::Geometry::CRectF Engine::Geometry::CRectF::operator *(const CRectF &r) const
 {
 return CRectF(TopLeft.Max(r.TopLeft),
 BottomRight.Min(r.BottomRight));
 }
 
 Engine::Geometry::CRectF Engine::Geometry::CRectF::operator +(const CRectF &r) const
 {
 return CRectF(TopLeft.Min(r.TopLeft),
 BottomRight.Max(r.BottomRight));
 }
 */

Engine::CArchive &operator<<(Engine::CArchive &ar, Engine::Geometry::CRectF r)
{
	ar << r.Left;
	ar << r.Top;
	ar << r.Right;
	ar << r.Bottom;
	
	return ar;
}

Engine::CArchive &operator>>(Engine::CArchive &ar, Engine::Geometry::CRectF &r)
{
	ar >> r.Left;
	ar >> r.Top;
	ar >> r.Right;
	ar >> r.Bottom;
	
	return ar;
}

/*
 void Engine::Geometry::CBezierSpline::Draw(Graphics::CLinePipe &LinePipe, int Steps)
 {
 int nKnots = GetNumKnots();
 
 if (nKnots < 0)
 return;
 
 if (Steps <= 0)
 Steps = BEZIER_SPLINE_SUBDIVISIONS;
 
 int nSegments = nKnots;
 
 if (!GetClosed())
 nSegments --;
 
 LinePipe.BeginStrip();
 
 Geometry::CVector3 Start = GetKnot(0);
 
 LinePipe.Vertex(Start.x, Start.y);
 
 for (int i = 0; i < nSegments; i ++)
 {
 for (int j = 0; j < Steps; j ++)
 {
 Geometry::CVector3 p;
 
 PointOnSpline(i, p, 1.0f / Steps * (j + 1));
 
 LinePipe.Vertex(p.x, p.y);
 }
 }
 
 LinePipe.End();
 }
 */


float Engine::Geometry::CVector2::GetLength() const /* throw() */
{
    return (float) sqrt(x * x + y * y);
}

float Engine::Geometry::CVector2::GetDistance(const CVector2 &v) const /* throw() */
{
    float dx = x - v.x;
    float dy = y - v.y;
	
    return (float) sqrt(dx * dx + dy * dy);
}

float Engine::Geometry::CVector2::GetDistanceSquared(const CVector2 &v) const /* throw() */
{
    float dx = x - v.x;
    float dy = y - v.y;
	
    return dx * dx + dy * dy;
}

void Engine::Geometry::CVector2::Normalize() /* throw() */
{
    float len = (float) sqrt(x * x + y * y);
	
    if (len > 1e-5)
    {
        float inv_len = 1.0f / len;
		
        x *= inv_len;
        y *= inv_len;
    }
}

void Engine::Geometry::CVector2::Rotate(float Angle) /* throw() */
{
    float s = (float) sin(Angle);
    float c = (float) cos(Angle);
	
    float xn = x * c - y * s;
	y  = x * s + y * c;
	
    x = xn;
}

Engine::Geometry::CVector2 Engine::Geometry::CVector2::Middle(const CVector2 &v) const /* throw() */
{
    return CVector2((v.x + x) * 0.5f,
                    (v.y + y) * 0.5f);
}

Engine::Geometry::CVector2 Engine::Geometry::CVector2::Lerp(const CVector2 &v, float t) const /* throw() */
{
    return CVector2(x + (v.x - x) * t,
                    y + (v.y - y) * t);
}

Engine::Geometry::CVector2 Engine::Geometry::CVector2::Min(const CVector2 &v) const /* throw() */
{
    return CVector2(x < v.x ? x : v.x,
                    y < v.y ? y : v.y);
}

Engine::Geometry::CVector2 Engine::Geometry::CVector2::Max(const CVector2 &v) const /* throw() */
{
    return CVector2(x > v.x ? x : v.x,
                    y > v.y ? y : v.y);
}

bool Engine::Geometry::CVector2::operator==(const CVector2 &v) const /* throw() */
{
    return x == v.x && y == v.y;
}

bool Engine::Geometry::CVector2::operator!=(const CVector2 &v) const /* throw() */
{
    return !(x == v.x && y == v.y);
}

Engine::Geometry::CVector3 Engine::Geometry::CVector3::Cross(const CVector3 &v) const /* throw() */
{
    return CVector3(y * v.z - z * v.y,
                    x * v.z - z * v.x,
                    x * v.y - y * v.x);
}

float Engine::Geometry::CVector3::GetLength() const /* throw() */
{
    return (float) sqrt(x * x + y * y + z * z);
}

float Engine::Geometry::CVector3::GetLengthSquared() const /* throw() */
{
    return x * x + y * y + z * z;
}

float Engine::Geometry::CVector3::GetDistance(const CVector3 &v) const /* throw() */
{
    float dx = x - v.x;
    float dy = y - v.y;
    float dz = z - v.z;
	
    return (float) sqrt(dx * dx + dy * dy + dz * dz);
}

float Engine::Geometry::CVector3::GetDistanceSquared(const CVector3 &v) const /* throw() */
{
    float dx = x - v.x;
    float dy = y - v.y;
    float dz = z - v.z;
	
    return dx * dx + dy * dy + dz * dz;
}

void Engine::Geometry::CVector3::Normalize() /* throw() */
{
    float len = (float) sqrt(x * x + y * y + z * z);
	
    if (len > 1e-5)
    {
        float inv_len = 1.0f / len;
		
        x *= inv_len;
        y *= inv_len;
        z *= inv_len;
    }
}

Engine::Geometry::CVector3 Engine::Geometry::CVector3::Middle(const CVector3 &v) const /* throw() */
{
    return CVector3((v.x + x) * 0.5f,
                    (v.y + y) * 0.5f,
                    (v.z + z) * 0.5f);
}

Engine::Geometry::CVector3 Engine::Geometry::CVector3::Lerp(const CVector3 &v, float t) const /* throw() */
{
    return CVector3(x + (v.x - x) * t,
                    y + (v.y - y) * t,
                    z + (v.z - z) * t);
}

Engine::Geometry::CVector3 Engine::Geometry::CVector3::Min(const CVector3 &v) const /* throw() */
{
    return CVector3(x < v.x ? x : v.x,
                    y < v.y ? y : v.y,
                    z < v.z ? z : v.z);
}

Engine::Geometry::CVector3 Engine::Geometry::CVector3::Max(const CVector3 &v) const /* throw() */
{
    return CVector3(x > v.x ? x : v.x,
                    y > v.y ? y : v.y,
                    z > v.z ? z : v.z);
}

Engine::CArchive &operator<<(Engine::CArchive &ar, Engine::Geometry::CVector3 v)
{
	ar << v.x;
	ar << v.y;
	ar << v.z;
	
	return ar;
}

Engine::CArchive &operator>>(Engine::CArchive &ar, Engine::Geometry::CVector3 &v)
{
	ar >> v.x;
	ar >> v.y;
	ar >> v.z;
	
	return ar;
}

float Engine::Geometry::CVector4::GetLength() const /* throw() */
{
    return (float) sqrt(x * x + y * y + z * z + w * w);
}

float Engine::Geometry::CVector4::GetDistance(const CVector4 &v) const /* throw() */
{
    float dx = x - v.x;
    float dy = y - v.y;
    float dz = z - v.z;
    float dw = w - v.w;
	
    return (float) sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

float Engine::Geometry::CVector4::GetDistanceSquared(const CVector4 &v) const /* throw() */
{
    float dx = x - v.x;
    float dy = y - v.y;
    float dz = z - v.z;
    float dw = w - v.w;
	
    return dx * dx + dy * dy + dz * dz + dw * dw;
}

void Engine::Geometry::CVector4::Normalize() /* throw() */
{
    float len = (float) sqrt(x * x + y * y + z * z + w * w);
	
    if (len > 1e-5)
    {
        float inv_len = 1.0f / len;
		
        x *= inv_len;
        y *= inv_len;
        z *= inv_len;
        w *= inv_len;
    }
}

Engine::Geometry::CVector4 Engine::Geometry::CVector4::Middle(const CVector4 &v) const /* throw() */
{
    return CVector4((v.x + x) * 0.5f,
                    (v.y + y) * 0.5f,
                    (v.z + z) * 0.5f,
                    (v.w + w) * 0.5f);
}

Engine::Geometry::CVector4 Engine::Geometry::CVector4::Lerp(const CVector4 &v, float t) const /* throw() */
{
    return CVector4(x + (v.x - x) * t,
                    y + (v.y - y) * t,
                    z + (v.z - z) * t,
                    w + (v.w - w) * t);
}

Engine::Geometry::CVector4 Engine::Geometry::CVector4::Min(const CVector4 &v) const /* throw() */
{
    return CVector4(x < v.x ? x : v.x,
                    y < v.y ? y : v.y,
                    z < v.z ? z : v.z,
                    w < v.w ? w : v.w);
}

Engine::Geometry::CVector4 Engine::Geometry::CVector4::Max(const CVector4 &v) const /* throw() */
{
    return CVector4(x > v.x ? x : v.x,
                    y > v.y ? y : v.y,
                    z > v.z ? z : v.z,
                    w > v.w ? w : v.w);
}

bool Engine::Geometry::CVector4::operator ==(const CVector4 &v) const /* throw() */
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Engine::Geometry::CVector4::operator !=(const CVector4 &v) const /* throw() */
{
    return !(x == v.x && y == v.y && z == v.z && w == v.w);
}

Engine::CArchive &operator<<(Engine::CArchive &ar, Engine::Geometry::CVector4 v)
{
	ar << v.x;
	ar << v.y;
	ar << v.z;
	ar << v.w;
	
	return ar;
}

Engine::CArchive &operator>>(Engine::CArchive &ar, Engine::Geometry::CVector4 &v)
{
	ar >> v.x;
	ar >> v.y;
	ar >> v.z;
	ar >> v.w;
	
	return ar;
}

Engine::Geometry::CAXYZ::CAXYZ(const Geometry::CQuat &_q)
{
	float angle = (float) acos(_q.w);
	float sa = (float) sin(angle);
	
	a = angle * 2.0f;
	
	if (fabs(sa) > 0.0001f)
	{
		sa = 1.0f / sa;
		v.x = _q.v.x * sa;
		v.y = _q.v.y * sa;
		v.z = _q.v.z * sa;
	}
	else
	{
		a   = 0.0f;
		v.x = 0.0f;
		v.y = 0.0f;
		v.z = 0.0f;
	}
}

Engine::Geometry::CQuat::CQuat(const Geometry::CAXYZ &_a)
{
	float a = _a.a * 0.5f;
	
	float sa = (float) sin(a);
	
	w   = (float) cos(a);
	v.x = _a.v.x * sa;
	v.y = _a.v.y * sa;
	v.z = _a.v.z * sa;
}

Engine::Geometry::CMatrix34 Engine::Geometry::CQuat::ToMatrix()
{
	CMatrix34 m;
	
	float x  = v.x;
	float x2 = x + x;
	float xx = x2 * x;
	
	float y  = v.y;
	float y2 = y + y;
	float yy = y2 * y;
	
	float z  = v.z;
	float z2 = z + z;
	float zz = z2 * z;
	
	float wx = w * x2;
	float wy = w * y2;
	float wz = w * z2;
	
	float xy = x * y2;
	float xz = x * z2;
	float yz = z * y2;
	
	m.m[0][0] = 1.0f - yy - zz;
	m.m[1][1] = 1.0f - xx - zz;
	m.m[2][2] = 1.0f - xx - yy;
	
	m.m[0][1] = xy + wz;
	m.m[1][0] = xy - wz;
	
	m.m[0][2] = xz - wy;
	m.m[2][0] = xz + wy;
	
	m.m[1][2] = yz + wx;
	m.m[2][1] = yz - wx;
	
	m.m[0][3] =
	m.m[1][3] =
	m.m[2][3] = 0.0f;
	
	return m;
}

Engine::Geometry::CMatrix34 Engine::Geometry::CQuat::ToInvMatrix()
{
	CMatrix34 m;
	
	float x  = v.x;
	float x2 = x + x;
	float xx = x2 * x;
	
	float y  = v.y;
	float y2 = y + y;
	float yy = y2 * y;
	
	float z  = v.z;
	float z2 = z + z;
	float zz = z2 * z;
	
	float wx = w * x2;
	float wy = w * y2;
	float wz = w * z2;
	
	float xy = x * y2;
	float xz = x * z2;
	float yz = z * y2;
	
	m.m[0][0] = 1.0f - yy - zz;
	m.m[1][1] = 1.0f - xx - zz;
	m.m[2][2] = 1.0f - xx - yy;
	
	m.m[0][1] = xy - wz;
	m.m[1][0] = xy + wz;
	
	m.m[0][2] = xz + wy;
	m.m[2][0] = xz - wy;
	
	m.m[1][2] = yz - wx;
	m.m[2][1] = yz + wx;
	
	m.m[0][3] =
	m.m[1][3] =
	m.m[2][3] = 0.0f;
	
	return m;
}

Engine::Geometry::CMatrix22::CMatrix22()
{
	memset(m, 0, sizeof(float[2][2]));
}

Engine::Geometry::CMatrix22::CMatrix22(float f00, float f01,
									   float f10, float f11)
{
	m[0][0] = f00; m[0][1] = f01;
	m[1][0] = f10; m[1][1] = f11;
}

Engine::Geometry::CMatrix22::CMatrix22(const CMatrix22 &mn)
{
	memcpy(m, mn.m, sizeof(float[2][2]));
}

void Engine::Geometry::CMatrix22::Zero()
{
	memset(m, 0, sizeof(float[2][2]));
}

void Engine::Geometry::CMatrix22::Identity()
{
	m[0][1] =
	m[1][0] = 0.0f;
	
	m[0][0] =
	m[1][1] = 1.0f;
}

void Engine::Geometry::CMatrix22::Rotate(float Angle)
{
	float s = (float) sin(Angle);
	float c = (float) cos(Angle);
	
	float mr[2][2];
	
	mr[0][0] =  m[0][0] * c +
	m[0][1] * s;
	
	mr[0][1] = -m[0][0] * s +
	m[0][1] * c;
	
	mr[1][0] =  m[1][0] * c +
	m[1][1] * s;
	
	mr[1][1] = -m[1][0] * s +
	m[1][1] * c;
	
	memcpy(m, mr, sizeof(float[2][2]));
}

void Engine::Geometry::CMatrix22::Rotation(float Angle)
{
	float s = (float) sin(Angle);
	float c = (float) cos(Angle);
	
	m[0][0] = c; m[0][1] = -s;
	m[1][0] = s; m[1][1] =  c;
}

void Engine::Geometry::CMatrix22::Scaling(const Geometry::CVector2 &v)
{
	m[0][0] = v.x;  m[0][1] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = v.y;
}

void Engine::Geometry::CMatrix22::Scaling(float ScaleX, float ScaleY)
{
	m[0][0] = ScaleX; m[0][1] = 0.0f;
	m[1][0] = 0.0f;   m[1][1] = ScaleY;
}

void Engine::Geometry::CMatrix22::Scale(const Geometry::CVector2 &v)
{
	m[0][0] *= v.x; m[0][1] *= v.x;
	m[1][0] *= v.y; m[1][1] *= v.y;
}

void Engine::Geometry::CMatrix22::Scale(float ScaleX, float ScaleY)
{
	m[0][0] *= ScaleX; m[0][1] *= ScaleX;
	m[1][0] *= ScaleY; m[1][1] *= ScaleY;
}

void Engine::Geometry::CMatrix22::ScaleCols(const CVector2 &v)
{
	m[0][0] *= v.x; m[0][1] *= v.y;
	m[1][0] *= v.x; m[1][1] *= v.y;
}

void Engine::Geometry::CMatrix22::ScaleCols(float ScaleX, float ScaleY)
{
	m[0][0] *= ScaleX; m[0][1] *= ScaleY;
	m[1][0] *= ScaleX; m[1][1] *= ScaleY;
}

Engine::Geometry::CMatrix22 Engine::Geometry::CMatrix22::Transpose() const
{
	return CMatrix22(m[0][0], m[1][0],
					 m[0][1], m[1][1]);
}

Engine::Geometry::CMatrix22 Engine::Geometry::CMatrix22::Inverse() const
{
	float det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
	
	if (fabs(det) < 1e-5)
		return CMatrix22(*this);
	
	float det1 = 1.0f / det;
	
	return CMatrix22( det1 * m[1][1], -det1 * m[0][1],
					 -det1 * m[1][0],  det1 * m[0][0]);
}

Engine::Geometry::CVector2 Engine::Geometry::CMatrix22::GetCol(int Col) const
{
	assert(Col>=0 && Col<=1);
	return CVector2(m[0][Col], m[1][Col]);
}

Engine::Geometry::CVector2 Engine::Geometry::CMatrix22::GetRow(int Row) const
{
	assert(Row>=0 && Row<=1);
	return CVector2(m[Row][0], m[Row][1]);
}

Engine::Geometry::CVector2 Engine::Geometry::CMatrix22::operator *(const CVector2 &v) const
{
	return CVector2(m[0][0] * v.x + m[0][1] * v.y,
					m[1][0] * v.x + m[1][1] * v.y);
}

Engine::Geometry::CMatrix22 Engine::Geometry::CMatrix22::operator *(const CMatrix22 &mn) const
{
	return CMatrix22(m[0][0] * mn.m[0][0] +
					 m[0][1] * mn.m[1][0],
					 
					 m[0][0] * mn.m[0][1] +
					 m[0][1] * mn.m[1][1],
					 
					 m[1][0] * mn.m[0][0] +
					 m[1][1] * mn.m[1][0],
					 
					 m[1][0] * mn.m[0][1] +
					 m[1][1] * mn.m[1][1]);
}

Engine::Geometry::CMatrix22::operator Engine::Geometry::CMatrix23() const
{
	return CMatrix23(m[0][0], m[0][1], 0.0f,
					 m[1][0], m[1][1], 0.0f);
}

Engine::CArchive &operator<<(Engine::CArchive &ar, Engine::Geometry::CMatrix22 m)
{
	for (int i = 0; i < 2; i ++)
		for (int j = 0; j < 2; j ++)
			ar << m.m[i][j];
	
	return ar;
}

Engine::CArchive &operator>>(Engine::CArchive &ar, Engine::Geometry::CMatrix22 &m)
{
	for (int i = 0; i < 2; i ++)
		for (int j = 0; j < 2; j ++)
			ar >> m.m[i][j];
	
	return ar;
}

Engine::Geometry::CMatrix23::CMatrix23()
{
	memset(m, 0, sizeof(float[2][3]));
}

Engine::Geometry::CMatrix23::CMatrix23(float f00, float f01, float f02,
									   float f10, float f11, float f12)
{
	m[0][0] = f00; m[0][1] = f01; m[0][2] = f02;
	m[1][0] = f10; m[1][1] = f11; m[1][2] = f12;
}

Engine::Geometry::CMatrix23::CMatrix23(const CMatrix23 &mn)
{
	memcpy(m, mn.m, sizeof(float[2][3]));
}

void Engine::Geometry::CMatrix23::Ortho(float Left, float Top, float Right, float Bottom, float ViewportWidth, float ViewportHeight)
{
	Geometry::CVector2 Scale(ViewportWidth / (Right - Left), ViewportHeight / (Bottom - Top));
	
	Identity();
	this->Scale(Scale.x, Scale.y);
	PreTranslate(Left, Top);
}

void Engine::Geometry::CMatrix23::Zero()
{
	memset(m, 0, sizeof(float[2][3]));
}

void Engine::Geometry::CMatrix23::Identity()
{
	memset(m, 0, sizeof(float[2][3]));
	
	m[0][0] =
	m[1][1] = 1.0f;
}

void Engine::Geometry::CMatrix23::Scaling(const CVector2 &v)
{
	memset(m, 0, sizeof(float[2][3]));
	
	m[0][0] = v.x;
	m[1][1] = v.y;
}

void Engine::Geometry::CMatrix23::Scaling(float ScaleX, float ScaleY)
{
	memset(m, 0, sizeof(float[2][3]));
	
	m[0][0] = ScaleX;
	m[1][1] = ScaleY;
}

void Engine::Geometry::CMatrix23::Rotation(float Angle)
{
	float s = (float) sin(Angle);
	float c = (float) cos(Angle);
	
	m[0][0] = c; m[0][1] = -s; m[0][2] = 0.0f;
	m[1][0] = s; m[1][1] =  c; m[1][2] = 0.0f;
}

void Engine::Geometry::CMatrix23::Scale(const CVector2 &v)
{
	m[0][0] *= v.x; m[0][1] *= v.x; m[0][2] *= v.x;
	m[1][0] *= v.y; m[1][1] *= v.y; m[1][2] *= v.y;
}

void Engine::Geometry::CMatrix23::Scale(float ScaleX, float ScaleY)
{
	m[0][0] *= ScaleX; m[0][1] *= ScaleX; m[0][2] *= ScaleX;
	m[1][0] *= ScaleY; m[1][1] *= ScaleY; m[1][2] *= ScaleY;
}

void Engine::Geometry::CMatrix23::ScaleCols(const CVector3 &v)
{
	m[0][0] *= v.x; m[0][1] *= v.y; m[0][2] *= v.z;
	m[1][0] *= v.x; m[1][1] *= v.y; m[1][2] *= v.z;
}

void Engine::Geometry::CMatrix23::ScaleCols(float ScaleX, float ScaleY, float ScaleZ)
{
	m[0][0] *= ScaleX; m[0][1] *= ScaleY; m[0][2] *= ScaleZ;
	m[1][0] *= ScaleX; m[1][1] *= ScaleY; m[1][2] *= ScaleZ;
}

void Engine::Geometry::CMatrix23::Rotate(float Angle)
{
	float s = (float) sin(Angle);
	float c = (float) cos(Angle);
	
	float mr[2][3];
	
	mr[0][0] =  m[0][0] * c +
	m[0][1] * s;
	
	mr[0][1] = -m[0][0] * s +
	m[0][1] * c;
	
	mr[1][0] =  m[1][0] * c +
	m[1][1] * s;
	
	mr[1][1] = -m[1][0] * s +
	m[1][1] * c;
	
	mr[0][2] = m[0][2];
	mr[1][2] = m[1][2];
	
	memcpy(m, mr, sizeof(float[2][3]));
}

Engine::Geometry::CMatrix23 Engine::Geometry::CMatrix23::Transpose() const
{
	return CMatrix23(m[0][0], m[1][0], m[0][2],
					 m[0][1], m[1][1], m[1][2]);
}

void Engine::Geometry::CMatrix23::Translate(const CVector2 &v)
{
	m[0][2] += v.x;
	m[1][2] += v.y;
}

void Engine::Geometry::CMatrix23::Translate(float x, float y)
{
	m[0][2] += x;
	m[1][2] += y;
}

void Engine::Geometry::CMatrix23::PreTranslate(const CVector2 &v)
{
	m[0][2] -= m[0][0] * v.x + m[0][1] * v.y;
	m[1][2] -= m[1][0] * v.x + m[1][1] * v.y;
}

void Engine::Geometry::CMatrix23::PreTranslate(float x, float y)
{
	m[0][2] -= m[0][0] * x + m[0][1] * y;
	m[1][2] -= m[1][0] * x + m[1][1] * y;
}

Engine::Geometry::CVector2 Engine::Geometry::CMatrix23::GetCol(int Col) const
{
	assert(Col>=0 && Col<=2);
	return CVector2(m[0][Col], m[1][Col]);
}

Engine::Geometry::CVector3 Engine::Geometry::CMatrix23::GetRow(int Row) const
{
	assert(Row>=0 && Row<=1);
	return CVector3(m[Row][0], m[Row][1], m[Row][2]);
}

Engine::Geometry::CMatrix23 Engine::Geometry::CMatrix23::Inverse() const
{
	float det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
	
	if (fabs(det) < 1e-5)
		return CMatrix23(*this);
	
	float det1 = 1.0f / det;
	
	float mi[2][2];
	
	mi[0][0] =  det1 * m[1][1];
	mi[0][1] = -det1 * m[0][1];
	mi[1][0] = -det1 * m[1][0];
	mi[1][1] =  det1 * m[0][0];
	
	return CMatrix23(mi[0][0], mi[0][1], -(mi[0][0] * m[0][2] + mi[0][1] * m[1][2]),
					 mi[1][0], mi[1][1], -(mi[1][0] * m[0][2] + mi[1][1] * m[1][2]));
}

Engine::Geometry::CVector2 Engine::Geometry::CMatrix23::operator *(const CVector2 &v) const
{
	return CVector2(m[0][0] * v.x + m[0][1] * v.y + m[0][2],
					m[1][0] * v.x + m[1][1] * v.y + m[1][2]);
}

Engine::Geometry::CMatrix23 Engine::Geometry::CMatrix23::operator *(const CMatrix23 &mn) const
{
	CMatrix23 r;
	int i, j;
	
	for (i = 0; i < 2; i++)
		for (j = 0; j < 3; j++)
			r.m[i][j] = m[i][0] * mn.m[0][j] +
			m[i][1] * mn.m[1][j] +
			m[i][2] * ((j == 2) ? 1.0f : 0.0f);
	
	return r;
}

Engine::Geometry::CMatrix23::operator Engine::Geometry::CMatrix22() const
{
	return CMatrix22(m[0][0], m[0][1],
					 m[1][0], m[1][1]);
}

Engine::CArchive &operator<<(Engine::CArchive &ar, Engine::Geometry::CMatrix23 m)
{
	for (int i = 0; i < 2; i ++)
		for (int j = 0; j < 3; j ++)
			ar << m.m[i][j];
	
	return ar;
}

Engine::CArchive &operator>>(Engine::CArchive &ar, Engine::Geometry::CMatrix23 &m)
{
	for (int i = 0; i < 2; i ++)
		for (int j = 0; j < 3; j ++)
			ar >> m.m[i][j];
	
	return ar;
}

Engine::Geometry::CMatrix33::CMatrix33()
{
	memset(m, 0, sizeof(float[3][3]));
}

Engine::Geometry::CMatrix33::CMatrix33(float f00, float f01, float f02,
									   float f10, float f11, float f12,
									   float f20, float f21, float f22)
{
	m[0][0] = f00; m[0][1] = f01; m[0][2] = f02;
	m[1][0] = f10; m[1][1] = f11; m[1][2] = f12;
	m[2][0] = f20; m[2][1] = f21; m[2][2] = f22;
}

Engine::Geometry::CMatrix33::CMatrix33(const CMatrix33 &mn)
{
	memcpy(m, mn.m, sizeof(float[3][3]));
}

void Engine::Geometry::CMatrix33::Zero()
{
	memset(m, 0, sizeof(float[3][3]));
}

void Engine::Geometry::CMatrix33::Identity()
{
	memset(m, 0, sizeof(float[3][3]));
	
	m[0][0] =
	m[1][1] =
	m[2][2] = 1.0f;
}

void Engine::Geometry::CMatrix33::Scaling(const CVector3 &v)
{
	memset(m, 0, sizeof(float[3][3]));
	
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;
}

void Engine::Geometry::CMatrix33::Scaling(float ScaleX, float ScaleY, float ScaleZ)
{
	memset(m, 0, sizeof(float[3][3]));
	
	m[0][0] = ScaleX;
	m[1][1] = ScaleY;
	m[2][2] = ScaleZ;
}

void Engine::Geometry::CMatrix33::RotationX(float Angle)
{
	float sa = (float) sin(Angle);
	float ca = (float) cos(Angle);
	
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = ca;   m[1][2] = -sa;
	m[2][0] = 0.0f; m[2][1] = sa;   m[2][2] = ca;
}

void Engine::Geometry::CMatrix33::RotationY(float Angle)
{
	float sa = (float) sin(Angle);
	float ca = (float) cos(Angle);
	
	m[0][0] = ca;   m[0][1] = 0.0f; m[0][2] = sa;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] = -sa;  m[2][1] = 0.0f; m[2][2] = ca;
}

void Engine::Geometry::CMatrix33::RotationZ(float Angle)
{
	float sa = (float) sin(Angle);
	float ca = (float) cos(Angle);
	
	m[0][0] = ca;   m[0][1] = -sa;  m[0][2] = 0.0f;
	m[1][0] = sa;   m[1][1] = ca;   m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f;
}

void Engine::Geometry::CMatrix33::Scale(const CVector3 &v)
{
	m[0][0] *= v.x; m[0][1] *= v.x; m[0][2] *= v.x;
	m[1][0] *= v.y; m[1][1] *= v.y; m[1][2] *= v.y;
	m[2][0] *= v.z; m[2][1] *= v.z; m[2][2] *= v.z;
}

void Engine::Geometry::CMatrix33::Scale(float ScaleX, float ScaleY, float ScaleZ)
{
	m[0][0] *= ScaleX; m[0][1] *= ScaleX; m[0][2] *= ScaleX;
	m[1][0] *= ScaleY; m[1][1] *= ScaleY; m[1][2] *= ScaleY;
	m[2][0] *= ScaleZ; m[2][1] *= ScaleZ; m[2][2] *= ScaleZ;
}

void Engine::Geometry::CMatrix33::ScaleCols(const CVector3 &v)
{
	m[0][0] *= v.x; m[0][1] *= v.y; m[0][2] *= v.z;
	m[1][0] *= v.x; m[1][1] *= v.y; m[1][2] *= v.z;
	m[2][0] *= v.x; m[2][1] *= v.y; m[2][2] *= v.z;
}

void Engine::Geometry::CMatrix33::ScaleCols(float ScaleX, float ScaleY, float ScaleZ)
{
	m[0][0] *= ScaleX; m[0][1] *= ScaleY; m[0][2] *= ScaleZ;
	m[1][0] *= ScaleX; m[1][1] *= ScaleY; m[1][2] *= ScaleZ;
	m[2][0] *= ScaleX; m[2][1] *= ScaleY; m[2][2] *= ScaleZ;
}

void Engine::Geometry::CMatrix33::RotateX(float Angle)
{
	CMatrix33 Rot;
	Rot.RotationX(Angle);
	*this = *this * Rot;
}

void Engine::Geometry::CMatrix33::RotateY(float Angle)
{
	CMatrix33 Rot;
	Rot.RotationY(Angle);
	*this = *this * Rot;
}

void Engine::Geometry::CMatrix33::RotateZ(float Angle)
{
	CMatrix33 Rot;
	Rot.RotationZ(Angle);
	*this = *this * Rot;
}

void Engine::Geometry::CMatrix33::PreRotateX(float Angle)
{
	CMatrix33 Rot;
	Rot.RotationX(Angle);
	*this = Rot * *this ;
}

void Engine::Geometry::CMatrix33::PreRotateY(float Angle)
{
	CMatrix33 Rot;
	Rot.RotationY(Angle);
	*this = Rot * *this ;
}

void Engine::Geometry::CMatrix33::PreRotateZ(float Angle)
{
	CMatrix33 Rot;
	Rot.RotationZ(Angle);
	*this = Rot * *this ;
}

Engine::Geometry::CMatrix33 Engine::Geometry::CMatrix33::Transpose() const
{
	return CMatrix33(m[0][0], m[1][0], m[2][0],
					 m[0][1], m[1][1], m[2][1],
					 m[0][2], m[1][2], m[2][2]);
}

Engine::Geometry::CVector3 Engine::Geometry::CMatrix33::GetCol(int Col) const
{
	assert(Col>=0 && Col<=2);
	return CVector3(m[0][Col], m[1][Col], m[2][Col]);
}

Engine::Geometry::CVector3 Engine::Geometry::CMatrix33::GetRow(int Row) const
{
	assert(Row>=0 && Row<=2);
	return CVector3(m[Row][0], m[Row][1], m[Row][2]);
}

Engine::Geometry::CMatrix33 Engine::Geometry::CMatrix33::Inverse() const
{
	float det = -m[0][0] * m[1][1] * m[2][2] + m[0][0] * m[1][2] * m[2][1] +
	m[1][0] * m[0][1] * m[2][2] - m[1][0] * m[0][2] * m[2][1] -
	m[2][0] * m[0][1] * m[1][2] + m[2][0] * m[0][2] * m[1][1];
	
	if (fabs(det) < 1e-5)
		return CMatrix33(*this);
	
	float det1 = 1.0f / det;
	
	float mi[3][3];
	
	float dm00 = det1 * m[0][0];
	float dm01 = det1 * m[0][1];
	float dm02 = det1 * m[0][2];
	float dm10 = det1 * m[1][0];
	float dm11 = det1 * m[1][1];
	float dm12 = det1 * m[1][2];
	
	mi[0][0] = dm12 * m[2][1] - dm11 * m[2][2];
	mi[0][1] = dm01 * m[2][2] - dm02 * m[2][1];
	mi[0][2] = dm02 * m[1][1] - dm01 * m[1][2];
	
	mi[1][0] = dm10 * m[2][2] - dm12 * m[2][0];
	mi[1][1] = dm02 * m[2][0] - dm00 * m[2][2];
	mi[1][2] = dm00 * m[1][2] - dm02 * m[1][0];
	
	mi[2][0] = dm11 * m[2][0] - dm10 * m[2][1];
	mi[2][1] = dm00 * m[2][1] - dm01 * m[2][0];
	mi[2][2] = dm01 * m[1][0] - dm00 * m[1][1];
	
	return CMatrix33(mi[0][0], mi[0][1], mi[0][2],
					 mi[1][0], mi[1][1], mi[1][2],
					 mi[2][0], mi[2][1], mi[2][2]);
}

Engine::Geometry::CVector3 Engine::Geometry::CMatrix33::operator *(const CVector3 &v) const
{
	return CVector3(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
					m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
					m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
}

Engine::Geometry::CMatrix33 Engine::Geometry::CMatrix33::operator *(const CMatrix33 &mn) const
{
	CMatrix33 r;
	int i, j;
	
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			r.m[i][j] = m[i][0] * mn.m[0][j] +
			m[i][1] * mn.m[1][j] +
			m[i][2] * mn.m[2][j];
	
	return r;
}

Engine::Geometry::CMatrix33::operator Engine::Geometry::CMatrix34() const
{
	return CMatrix34(
					 m[0][0], m[0][1], m[0][2], 0.0f,
					 m[1][0], m[1][1], m[1][2], 0.0f,
					 m[2][0], m[2][1], m[2][2], 0.0f);
}

Engine::Geometry::CMatrix34::CMatrix34(const CMatrix23 &_m)
{
	m[0][0] = _m.m[0][0];
	m[0][1] = _m.m[0][1];
	m[0][2] = 0.0f;
	m[0][3] = _m.m[0][2];
	
	m[1][0] = _m.m[1][0];
	m[1][1] = _m.m[1][1];
	m[1][2] = 0.0f;
	m[1][3] = _m.m[1][2];
	
	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;
}

Engine::CArchive &operator<<(Engine::CArchive &ar, Engine::Geometry::CMatrix33 m)
{
	for (int i = 0; i < 3; i ++)
		for (int j = 0; j < 3; j ++)
			ar << m.m[i][j];
	
	return ar;
}

Engine::CArchive &operator>>(Engine::CArchive &ar, Engine::Geometry::CMatrix33 &m)
{
	for (int i = 0; i < 3; i ++)
		for (int j = 0; j < 3; j ++)
			ar >> m.m[i][j];
	
	return ar;
}

Engine::Geometry::CMatrix34::CMatrix34()
{
	memset(m, 0, sizeof(float[3][4]));
}

Engine::Geometry::CMatrix34::CMatrix34(float f00, float f01, float f02, float f03,
									   float f10, float f11, float f12, float f13,
									   float f20, float f21, float f22, float f23)
{
	m[0][0] = f00; m[0][1] = f01; m[0][2] = f02; m[0][3] = f03;
	m[1][0] = f10; m[1][1] = f11; m[1][2] = f12; m[1][3] = f13;
	m[2][0] = f20; m[2][1] = f21; m[2][2] = f22; m[2][3] = f23;
}

Engine::Geometry::CMatrix34::CMatrix34(const CMatrix34 &mn)
{
	memcpy(m, mn.m, sizeof(float[3][4]));
}

void Engine::Geometry::CMatrix34::Zero()
{
	memset(m, 0, sizeof(float[3][4]));
}

void Engine::Geometry::CMatrix34::Identity()
{
	memset(m, 0, sizeof(float[3][4]));
	
	m[0][0] =
	m[1][1] =
	m[2][2] = 1.0f;
}

void Engine::Geometry::CMatrix34::Scaling(const CVector3 &v)
{
	memset(m, 0, sizeof(float[3][4]));
	
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;
}

void Engine::Geometry::CMatrix34::Scaling(float ScaleX, float ScaleY, float ScaleZ)
{
	memset(m, 0, sizeof(float[3][4]));
	
	m[0][0] = ScaleX;
	m[1][1] = ScaleY;
	m[2][2] = ScaleZ;
}

void Engine::Geometry::CMatrix34::RotationX(float Angle)
{
	float sa = (float) sin(Angle);
	float ca = (float) cos(Angle);
	
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = ca;   m[1][2] = -sa;  m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = sa;   m[2][2] = ca;   m[2][3] = 0.0f;
}

void Engine::Geometry::CMatrix34::RotationY(float Angle)
{
	float sa = (float) sin(Angle);
	float ca = (float) cos(Angle);
	
	m[0][0] = ca;   m[0][1] = 0.0f; m[0][2] = sa;   m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = -sa;  m[2][1] = 0.0f; m[2][2] = ca;   m[2][3] = 0.0f;
}

void Engine::Geometry::CMatrix34::RotationZ(float Angle)
{
	float sa = (float) sin(Angle);
	float ca = (float) cos(Angle);
	
	m[0][0] = ca;   m[0][1] = -sa;  m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = sa;   m[1][1] = ca;   m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f; m[2][3] = 0.0f;
}

void Engine::Geometry::CMatrix34::Scale(const CVector3 &v)
{
	m[0][0] *= v.x; m[0][1] *= v.x; m[0][2] *= v.x; m[0][3] *= v.x;
	m[1][0] *= v.y; m[1][1] *= v.y; m[1][2] *= v.y; m[1][3] *= v.y;
	m[2][0] *= v.z; m[2][1] *= v.z; m[2][2] *= v.z; m[2][3] *= v.z;
}

void Engine::Geometry::CMatrix34::Scale(float ScaleX, float ScaleY, float ScaleZ)
{
	m[0][0] *= ScaleX; m[0][1] *= ScaleX; m[0][2] *= ScaleX; m[0][3] *= ScaleX;
	m[1][0] *= ScaleY; m[1][1] *= ScaleY; m[1][2] *= ScaleY; m[1][3] *= ScaleY;
	m[2][0] *= ScaleZ; m[2][1] *= ScaleZ; m[2][2] *= ScaleZ; m[2][3] *= ScaleZ;
}

void Engine::Geometry::CMatrix34::ScaleCols(const CVector4 &v)
{
	m[0][0] *= v.x; m[0][1] *= v.y; m[0][2] *= v.z; m[0][3] *= v.w;
	m[1][0] *= v.x; m[1][1] *= v.y; m[1][2] *= v.z; m[1][3] *= v.w;
	m[2][0] *= v.x; m[2][1] *= v.y; m[2][2] *= v.z; m[2][3] *= v.w;
}

void Engine::Geometry::CMatrix34::ScaleCols(float ScaleX, float ScaleY, float ScaleZ, float ScaleW)
{
	m[0][0] *= ScaleX; m[0][1] *= ScaleY; m[0][2] *= ScaleZ; m[0][3] *= ScaleW;
	m[1][0] *= ScaleX; m[1][1] *= ScaleY; m[1][2] *= ScaleZ; m[1][3] *= ScaleW;
	m[2][0] *= ScaleX; m[2][1] *= ScaleY; m[2][2] *= ScaleZ; m[2][3] *= ScaleW;
}

void Engine::Geometry::CMatrix34::RotateX(float Angle)
{
	CMatrix34 Rot;
	Rot.RotationX(Angle);
	*this = *this * Rot;
}

void Engine::Geometry::CMatrix34::RotateY(float Angle)
{
	CMatrix34 Rot;
	Rot.RotationY(Angle);
	*this = *this * Rot;
}

void Engine::Geometry::CMatrix34::RotateZ(float Angle)
{
	CMatrix34 Rot;
	Rot.RotationZ(Angle);
	*this = *this * Rot;
}

void Engine::Geometry::CMatrix34::PreRotateX(float Angle)
{
	CMatrix34 Rot;
	Rot.RotationX(Angle);
	*this = Rot * *this ;
}

void Engine::Geometry::CMatrix34::PreRotateY(float Angle)
{
	CMatrix34 Rot;
	Rot.RotationY(Angle);
	*this = Rot * *this ;
}

void Engine::Geometry::CMatrix34::PreRotateZ(float Angle)
{
	CMatrix34 Rot;
	Rot.RotationZ(Angle);
	*this = Rot * *this ;
}

Engine::Geometry::CMatrix34 Engine::Geometry::CMatrix34::Transpose() const
{
	return CMatrix34(m[0][0], m[1][0], m[2][0], m[0][3],
					 m[0][1], m[1][1], m[2][1], m[1][3],
					 m[0][2], m[1][2], m[2][2], m[2][3]);
}

void Engine::Geometry::CMatrix34::Translate(const CVector3 &v)
{
	m[0][3] += v.x;
	m[1][3] += v.y;
	m[2][3] += v.z;
}

void Engine::Geometry::CMatrix34::Translate(float x, float y, float z)
{
	m[0][3] += x;
	m[1][3] += y;
	m[2][3] += z;
}

void Engine::Geometry::CMatrix34::PreTranslate(const CVector3 &v)
{
	m[0][3] -= m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
	m[1][3] -= m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
	m[2][3] -= m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
}

void Engine::Geometry::CMatrix34::PreTranslate(float x, float y, float z)
{
	m[0][3] -= m[0][0] * x + m[0][1] * y + m[0][2] * z;
	m[1][3] -= m[1][0] * x + m[1][1] * y + m[1][2] * z;
	m[2][3] -= m[2][0] * x + m[2][1] * y + m[2][2] * z;
}

Engine::Geometry::CVector3 Engine::Geometry::CMatrix34::GetCol(int Col) const
{
	assert(Col>=0 && Col<=3);
	return CVector3(m[0][Col], m[1][Col], m[2][Col]);
}

Engine::Geometry::CVector4 Engine::Geometry::CMatrix34::GetRow(int Row) const
{
	assert(Row>=0 && Row<=2);
	return CVector4(m[Row][0], m[Row][1], m[Row][2], m[Row][3]);
}

Engine::Geometry::CMatrix34 Engine::Geometry::CMatrix34::Inverse() const
{
	float det = -m[0][0] * m[1][1] * m[2][2] + m[0][0] * m[1][2] * m[2][1] +
	m[1][0] * m[0][1] * m[2][2] - m[1][0] * m[0][2] * m[2][1] -
	m[2][0] * m[0][1] * m[1][2] + m[2][0] * m[0][2] * m[1][1];
	
	if (fabs(det) < 1e-5)
		return CMatrix34(*this);
	
	float det1 = 1.0f / det;
	
	float mi[3][3];
	
	float dm00 = det1 * m[0][0];
	float dm01 = det1 * m[0][1];
	float dm02 = det1 * m[0][2];
	float dm10 = det1 * m[1][0];
	float dm11 = det1 * m[1][1];
	float dm12 = det1 * m[1][2];
	
	mi[0][0] = dm12 * m[2][1] - dm11 * m[2][2];
	mi[0][1] = dm01 * m[2][2] - dm02 * m[2][1];
	mi[0][2] = dm02 * m[1][1] - dm01 * m[1][2];
	
	mi[1][0] = dm10 * m[2][2] - dm12 * m[2][0];
	mi[1][1] = dm02 * m[2][0] - dm00 * m[2][2];
	mi[1][2] = dm00 * m[1][2] - dm02 * m[1][0];
	
	mi[2][0] = dm11 * m[2][0] - dm10 * m[2][1];
	mi[2][1] = dm00 * m[2][1] - dm01 * m[2][0];
	mi[2][2] = dm01 * m[1][0] - dm00 * m[1][1];
	
	return CMatrix34(mi[0][0], mi[0][1], mi[0][2], -(mi[0][0] * m[0][3] + mi[0][1] * m[1][3] + mi[0][2] * m[2][3]),
					 mi[1][0], mi[1][1], mi[1][2], -(mi[1][0] * m[0][3] + mi[1][1] * m[1][3] + mi[1][2] * m[2][3]),
					 mi[2][0], mi[2][1], mi[2][2], -(mi[2][0] * m[0][3] + mi[2][1] * m[1][3] + mi[2][2] * m[2][3]));
}

Engine::Geometry::CVector3 Engine::Geometry::CMatrix34::operator *(const CVector3 &v) const
{
	return CVector3(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
					m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
					m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
}

Engine::Geometry::CMatrix34 Engine::Geometry::CMatrix34::operator *(const CMatrix34 &mn) const
{
	CMatrix34 r;
	int i, j;
	
	for (i = 0; i < 3; i++)
		for (j = 0; j < 4; j++)
			r.m[i][j] = m[i][0] * mn.m[0][j] +
			m[i][1] * mn.m[1][j] +
			m[i][2] * mn.m[2][j] +
			m[i][3] * ((j == 3) ? 1.0f : 0.0f);
	
	return r;
}

Engine::Geometry::CMatrix34::operator Engine::Geometry::CMatrix33() const
{
	return CMatrix33(m[0][0], m[0][1], m[0][2],
					 m[1][0], m[1][1], m[1][2],
					 m[2][0], m[2][1], m[2][2]);
}

void Engine::Geometry::CMatrix34::SetCol(int Col, CVector3 &v)
{
	assert(Col >= 0 && Col <= 3);
	
	m[0][Col] = v.x;
	m[1][Col] = v.y;
	m[2][Col] = v.z;
}

void Engine::Geometry::CMatrix34::SetRow(int Row, CVector4 &v)
{
	assert(Row >= 0 && Row <= 2);
	
	m[Row][0] = v.x;
	m[Row][1] = v.y;
	m[Row][2] = v.z;
	m[Row][3] = v.w;
}

Engine::CArchive &operator<<(Engine::CArchive &ar, Engine::Geometry::CMatrix34 m)
{
	for (int i = 0; i < 3; i ++)
		for (int j = 0; j < 4; j ++)
			ar << m.m[i][j];
	
	return ar;
}

Engine::CArchive &operator>>(Engine::CArchive &ar, Engine::Geometry::CMatrix34 &m)
{
	for (int i = 0; i < 3; i ++)
		for (int j = 0; j < 4; j ++)
			ar >> m.m[i][j];
	
	return ar;
}

