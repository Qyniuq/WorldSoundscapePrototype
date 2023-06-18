#pragma once
#include <vector>
#include <map>
#include "Sound.h"

enum instrument_type {JaguarGuitar = 0, FemaleVoice};

class Instrument
{
private:
	std::string folder_direction;
	ALuint effectSlot;
	std::vector<int> octaves;

public:

	int min;
	int max;
	int sleep_value;
	instrument_type type;

	std::map <int, Sound> sounds;

	Instrument() = default;
	Instrument(std::string folder_dir, int min, int max, std::vector<int> oct, int sleep_val, const instrument_type&, const ALuint& effectSlot);

	void Play(int n);
	std::map<int, Sound> LoadSounds();
};
