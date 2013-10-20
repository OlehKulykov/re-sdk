
/* Revision 1.2.4 */

//#include "StdAfx.H"
#include "BaseTypes.h"

void Engine::CColor::GetHSL( float &h, float &l, float &s )
{
        float i, a, d, ia;

        if (r < g)
                i = r, a = g;
        else    i = g, a = r;

        if (a < b)
                a = b;

        if (i > b)
                i = b;

        d = a - i;

        if (d == 0.0f)
        {
                h = 0.0f;
                s = 0.0f;
                l = r;
        }
        else
        {
                if (r == a)
                        h = (g - b) / d;
                else if (g == a)
                        h = (b - r) / d + 2.0f;
                else    h = (r - g) / d + 4.0f;

                if (h < 0.0f)
                        h += 6.0f;

                h /= 6.0f;

                ia = i + a;
                s = (a - i) / (2.0f - ia);
                l = ia * 0.5f;
        }
}

void Engine::CColor::FromHSL( float h, float l, float s )
{
        float v, c, u, d;
        float i, a, il;
        int t;

        v = h * 6.0f;
        t = (int) v;
        c = v - t;

        il = 1.0f - l;

        a = l + il * s;
        i = l - il * s;

        if (a > 1.0f)
        {
                a = 1.0f;
                i = l * 2.0f - 1.0f;
        }

        if (i < 0.0f)
        {
                a = l * 2.0f;
                i = 0.0f;
        }

        d = a * (1.0f - c) + i * c;
        u = i * (1.0f - c) + a * c;

        switch (t)
        {
        case 0: r = a; g = u; b = i; break;
        case 1: r = d; g = a; b = i; break;
        case 2: r = i; g = a; b = u; break;
        case 3: r = i; g = d; b = a; break;
        case 4: r = u; g = i; b = a; break;
        case 5: r = a; g = i; b = d; break;
        }
}

void Engine::CColor::FromDWord( REUInt32 d )
{
        r = FromByteToFloat((BYTE) ( d        & 0xff));
        g = FromByteToFloat((BYTE) ((d >>  8) & 0xff));
        b = FromByteToFloat((BYTE) ((d >> 16) & 0xff));
        a = FromByteToFloat((BYTE) ((d >> 24)       ));
}

REUInt32 Engine::CColor::GetDWord() const
{
        REUInt32 dr = ((REUInt32) FromFloatToByte(r))      ;
        REUInt32 dg = ((REUInt32) FromFloatToByte(g)) <<  8;
        REUInt32 db = ((REUInt32) FromFloatToByte(b)) << 16;
        REUInt32 da = ((REUInt32) FromFloatToByte(a)) << 24;

        return dr | dg | db | da;
}

void Engine::CColor::Validate()
{
        if (r < 0.0f) r = 0.0f; else if (r > 1.0f) r = 1.0f;
        if (g < 0.0f) g = 0.0f; else if (g > 1.0f) g = 1.0f;
        if (b < 0.0f) b = 0.0f; else if (b > 1.0f) b = 1.0f;
        if (a < 0.0f) a = 0.0f; else if (a > 1.0f) a = 1.0f;
}
