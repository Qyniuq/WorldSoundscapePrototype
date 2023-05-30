#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <AL/efx.h>
#include <AL/alext.h>
#include <thread>
#include <string>
#include <limits>
#include <atomic>
#include <sstream>
#include <cstring>

extern std::atomic<bool> stop_flag;

class Sound {

private:

    ALuint m_buffer[3];
    ALuint m_source[3];
    ALuint effectSlot;

    std::string filename;

public:
    std::string flat_name{};
    std::string sharp_name{};

    int number{};
    int octave{};
    int variation;

    Sound() = default;

    Sound(const char* filename, std::string sharp_name, std::string flat_name, int number, int octave, const ALuint& effectSlot);

    Sound(const Sound& other);

    ~Sound();

    bool isPlaying(int n);

    void play(ALfloat y = 0.0f);

    int getVariation(int old_n);

    void stop(int n);

    const ALuint& getEffectSlot() const;
    std::string getFilename() const;

};

