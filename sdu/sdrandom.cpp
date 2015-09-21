#include "SDuPCH.h"
#include "sdrandom.h"
#include <time.h>
#include <cstdlib>

namespace Crown
{

    void SDSetSeed(uint32 seed)
    {
        srand(seed);
    }

    int32 SDRandom()
    {
        return rand();
    }

    uint32 SDRandom(uint32 range)
    {
		if (range > RAND_MAX)
		{
			return rand() / ((long)RAND_MAX + 1) * range;
		}
		return rand() % range;
    }

    float SDRandomFloat(float range)
    {
        return range * (1.0 * rand() / RAND_MAX);
    }

    CSDRandom::CSDRandom()
    {
        m_seed = (uint32)time(nullptr);
    }

    uint32 CSDRandom::Random()
    {
        uint32 next = m_seed;
        uint32 result;

        next *= 1103515245;
        next += 12345;
        result = (uint32) (next / 65536) % 2048;

        next *= 1103515245;
        next += 12345;
        result <<= 10;
        result ^= (uint32) (next / 65536) % 1024;

        next *= 1103515245;
        next += 12345;
        result <<= 10;
        result ^= (uint32) (next / 65536) % 1024;

        m_seed = next;
        return result;
    }


    uint32 CSDRandom::Random(uint32 unRange)
    {
        if ( unRange == 0 )
        {
            return 0;
        }
        return Random() % unRange;
    }

    uint32 CSDRandom::Random(uint32 unMinRange, uint32 unMaxRange)
    {
        if ( unMinRange == unMaxRange )
        {
            return unMaxRange;
        }

        return Random(unMaxRange - unMinRange + 1) + unMinRange;
    }
}

