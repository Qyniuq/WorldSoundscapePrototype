#pragma once
#include "Weather.h"
#include "Sound.h"
#include "Instrument.h"
#include "InstrumentInitializers.h"
#include "ScaleLibrary.h"
#include <ctime>
#include <thread>
#include <chrono>
#include <conio.h>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <iostream>

#define KEY_R 82
#define KEY_r 114
#define KEY_N 78
#define KEY_n 110
#define KEY_L 76
#define KEY_l 108
#define KEY_G 71
#define KEY_g 103
#define KEY_M 77
#define KEY_m 109
#define KEY_X 88
#define KEY_x 120

extern std::atomic<bool> stop_flag;

class WorldSoundscape
{
private:
	std::mutex mut;
	std::atomic<bool> random_location_update{ false };
	std::condition_variable cv;
	ALCdevice* device {alcOpenDevice(nullptr)};
	ALCcontext* context {alcCreateContext(device, nullptr)};
	ALuint reverbEffect;
	ALuint reverbEffectSlot;
	Instrument Jaguar;
	Instrument FemaleVoice;
	std::vector<notes> guitar_mode;
	std::vector<notes> female_voice_mode;

public:
	Weather weather;
	bool exit_World_Soundscape {false};
	WorldSoundscape();
	~WorldSoundscape();
	void setRandomLocation();
	void setUserEnterLocation();
	void setUserlocation();
	void mainMenu();
	void displayWeather(Weather& weather, std::vector<std::string>& notes_played);
	void updateWeather(Weather& weather);
	void updateScale();
	std::vector<notes> getMode(Instrument&);
	void play_notes(Instrument& instrument, Weather& weather, std::vector<std::string>& notes_played);
	void initMusic();
	void keyboard_listener();
};

