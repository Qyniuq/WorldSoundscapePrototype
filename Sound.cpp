#include "Sound.h"

Sound::Sound(const char* filename_input, std::string sharp_name, std::string flat_name, int number, int octave, const ALuint& effectSlot)
    : effectSlot{ effectSlot }, filename(filename_input), flat_name{ flat_name }, sharp_name{ sharp_name }, number{ number }, octave{ octave }, variation{ getVariation(Sound::variation) }
{
    for (size_t i{ 0 }; i < 3; ++i) {

        SF_INFO info{};
        SNDFILE* file{};

        std::stringstream filepath;
        filepath << filename_input << i + 1 << ".wav";

        file = sf_open(filepath.str().c_str(), SFM_READ, &info);

        if (!file) {
            std::cerr << "\nError opening file " << filepath.str() << " ";
        }
        else {
            std::vector<short> data(info.frames * info.channels);
            sf_read_short(file, data.data(), data.size());
            alGenBuffers(1, &m_buffer[i]);
            alBufferData(m_buffer[i], info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data.data(), data.size() * sizeof(short), info.samplerate);
            alGenSources(1, &m_source[i]);
            alSourcei(m_source[i], AL_BUFFER, m_buffer[i]);
            sf_close(file);

            //adds reverb to the source
            alSource3i(m_source[i], AL_AUXILIARY_SEND_FILTER, effectSlot, 1, AL_FILTER_NULL);
        }
    }
}

Sound::Sound(const Sound& other) : Sound((other.getFilename()).c_str(), other.flat_name, other.sharp_name, other.number, other.octave, other.getEffectSlot())
{
    std::cout << "\ncopy constructor used";
}

Sound::~Sound()
{
    alDeleteSources(3, &m_source[0]);
    alDeleteBuffers(3, &m_buffer[0]);
}

bool Sound::isPlaying(int n)
{
    ALint state;
    alGetSourcei(m_source[n], AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void Sound::play(ALfloat y)
{
    //plays a different variation every time
    alSource3f(m_source[variation], AL_POSITION, y, 0.0, 0.0);

    if (isPlaying(variation)) {

        Sound::variation = Sound::variation == 2 ? 0 : Sound::variation + 1;
    }

    if (isPlaying(variation)) {

        Sound::variation = Sound::variation == 2 ? 0 : Sound::variation + 1;
    }

    if (isPlaying(variation))

    {
        std::cout << " [need more variations] ";
    }

    if (!isPlaying(variation) && !stop_flag) {
        alSourcePlay(m_source[variation]);
        Sound::variation = getVariation(Sound::variation);
        // std::cout << Sound::name << " ";
    }
}

int Sound::getVariation(int old_n)
{
    int new_n{};

    do {
        new_n = rand() % 3;
    } while (new_n == old_n);

    return new_n;
}

void Sound::stop(int n)
{
    alSourceStop(m_source[n]);
}

const ALuint& Sound::getEffectSlot() const
{
    return Sound::effectSlot;
}

std::string Sound::getFilename() const
{
    return Sound::filename;
}