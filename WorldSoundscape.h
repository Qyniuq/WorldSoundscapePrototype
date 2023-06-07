#pragma once
#include "Weather.h"
#include "Sound.h"
#include "Instrument.h"
#include "ScaleLibrary.h"
#include <ctime>
#include <thread>
#include <chrono>
#include <conio.h>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <iostream>

extern std::atomic<bool> stop_flag;

class WorldSoundscape
{
private:
	bool paused {false};
	std::shared_mutex shmtx;
	std::shared_mutex update_mtx;
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
	void waitForCitiesListLoading();
	void mainMenu();
	void displayWeather(Weather& weather, std::vector<std::string>& notes_played);
	void updateWeather(Weather& weather);
	void updateScale();
	std::vector<notes> getMode(Instrument&);
	void play_notes(Instrument& instrument, Weather& weather, std::vector<std::string>& notes_played);
	void initMusic();
	void keyboard_listener();
};

