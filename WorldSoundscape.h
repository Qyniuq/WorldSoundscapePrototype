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

struct Location {
	std::string city;
	std::string country_code;
	std::string state_code {"none"};
	double lon;
	double lat;

	Location(std::string n, std::string c, std::string s, double longitude, double latitude) 
		: city{ n }, country_code{ c }, state_code{ s }, lon {longitude}, lat{ latitude } {}

	Location(std::string n, std::string c, double longitude, double latitude)
		: city{ n }, country_code{ c }, state_code{ "none" }, lon{ longitude }, lat{ latitude } {}

	friend std::ostream& operator<<(std::ostream& os, Location loc) {
		os << loc.city << " / " << loc.country_code << " || " << "lat: " << loc.lat << " / " << "lon: " << loc.lon;
		return os;
	}
};

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
	std::vector<Location> Saved_Locations;

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
	void favouriteLocationsMenu();
	void displayWeather(Weather& weather, std::vector<std::string>& notes_played);
	void updateWeather(Weather& weather);
	void updateScale();
	std::vector<notes> getMode(Instrument&);
	void play_notes(Instrument& instrument, Weather& weather, std::vector<std::string>& notes_played);
	void initMusic();
	void keyboard_listener();
};

