#include "InstrumentInitializers.h"

Instrument CreateJaguarGuitar(const ALuint& EffectSlot)
{
    std::vector<int>octaves{ 2, 3, 4, 5, 6 };

    const int D2 = -22;
    const int D6 = 26;

    Instrument ins("JaguarGuitar", D2, D6, octaves, 8000, JaguarGuitar, EffectSlot);

    return ins;
}

Instrument CreateFemaleVoice(const ALuint& EffectSlot)
{
    std::vector<int>octaves{ 3, 4, 5 };

    const int B3 = -1;
    const int D5 = 14;

    Instrument ins("FemaleVoice", B3, D5, octaves, 12000, FemaleVoice, EffectSlot);

    return ins;
}