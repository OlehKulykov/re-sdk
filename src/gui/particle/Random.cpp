/*
 * Copyright (c) 2002-2006 Fenomen Games.  All Rights Reserved.
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


/* Revision 1.1 */

//#include "StdAfx.H"
#include "Base.h"
#include "Random.h"
#include "Timer.h"


Engine::CRandom::CRandom()
{
        m_pRandTablePtr0 = m_RandTable;
        m_pRandTablePtr1 = m_RandTable + 1;

        Seed();
}

Engine::CRandom::CRandom(REUInt32 _Seed)
{
        m_pRandTablePtr0 = m_RandTable;
        m_pRandTablePtr1 = m_RandTable + 1;

        Seed(_Seed);
}

void Engine::CRandom::Seed(REUInt32 Seed)
{
        REUInt32 x = Seed;

        for (int i = 0; i < RANDOM_TABLE_SIZE; i ++)
        {
                m_RandTable[i] = (x + 341392369) ^ (x - 97832771);
                x = (x >> 7) | (x << 25);
                x += 1734929173;
        }
}

void Engine::CRandom::Seed()
{
        Seed((REUInt32) clock());
}

REUInt32 Engine::CRandom::GenRandDWord()
{
        REUInt32 v0 = *m_pRandTablePtr0;
        REUInt32 v1 = *m_pRandTablePtr1;

        v0 += v1;
        v0 = (v0 << 7) ^ (v1 >> 7);
        v1 += v0 * 371 + 3034907031;
        v0 += v1 * v1 * 317 + 2342812;

        *m_pRandTablePtr0 ^= v1;
        *m_pRandTablePtr1 += v0;

        do
        {
                m_pRandTablePtr1 ++;

                if (m_pRandTablePtr1 == m_RandTable + RANDOM_TABLE_SIZE)
                        m_pRandTablePtr1 = m_RandTable;
                else
                {
                        m_pRandTablePtr0 ++;

                        if (m_pRandTablePtr0 == m_RandTable + RANDOM_TABLE_SIZE)
                                m_pRandTablePtr0 = m_RandTable;
                }

        } while (m_pRandTablePtr0 == m_pRandTablePtr1);

        return v1;
}

double Engine::CRandom::GenRandDouble()
{
        return ((double) GenRandDWord()) / 4294967295.0;
}

REUInt32 Engine::CRandom::GetDWordRand()
{
        return GenRandDWord();
}

REUInt32 Engine::CRandom::GetDWordRandMinMax(REUInt32 Min, REUInt32 Max)
{
    assert(Min <= Max);

        if (Min == Max)
                return Min;

        return GenRandDWord() % (Max + 1 - Min) + Min;
}

int Engine::CRandom::GetIntRandMinMax(int Min, int Max)
{
        assert(Min <= Max);

        if (Min == Max)
                return Min;

        REUInt32 Range = static_cast<REUInt32>(Max - Min);

        return static_cast<int>(GenRandDWord() % Range) + Min;
}

bool Engine::CRandom::GetBoolRand()
{
        return (GenRandDWord() % 2 == 1);
}

REUInt32 Engine::CRandom::GetDWordRandMod(REUInt32 Mod)
{
        assert(Mod != 0);

        return GenRandDWord() % Mod;
}

float Engine::CRandom::GetFloatRand01()
{
        return (float) GenRandDouble();
}

float Engine::CRandom::GetFloatRand11()
{
        return (float) (GenRandDouble() * 2.0 - 1.0);
}

float Engine::CRandom::GetFloatRandMod(float Mod)
{
        return (float) (GenRandDouble() * Mod);
}

float Engine::CRandom::GetFloatRandMinMax(float Min, float Max)
{
        assert(Min <= Max);

        if (Min == Max)
                return Min;

        return (float) (GenRandDouble() * (Max - Min) + Min);
}

float Engine::CRandom::GetFloatRandAngle()
{
        return (float) (GenRandDouble() * M_2PI);
}
