#pragma once
#include <vector>
#include <map>
#include "Sound.h"

class Instrument
{
private:
	ALuint effectSlot;

	std::vector<int> octaves;

	std::string folder_direction;

public:
	
	int min;
	int max;

	int sleep_value;

	std::map <int, Sound> sounds;

	Instrument(std::string folder_dir, int min, int max, std::vector<int> oct, int sleep_val, const ALuint& effectSlot);

	std::map<int, Sound> LoadSounds();
};
