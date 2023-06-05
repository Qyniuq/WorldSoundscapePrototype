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
	std::shared_mutex shmtx;
	std::atomic<bool> random_location_update{ false };
	bool wait{ false };
	std::condition_variable cv;
	std::condition_variable cv2;
	std::condition_variable cv3;
	ALCdevice* device {alcOpenDevice(nullptr)};
	ALCcontext* context {alcCreateContext(device, nullptr)};
	ALuint reverbEffect;
	ALuint reverbEffectSlot;	
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
	void play_notes(Instrument& instrument, Weather& weather, std::vector<std::string>& notes_played);
	void initMusic();
	void keyboard_listener();
};

