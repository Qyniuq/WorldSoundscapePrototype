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
	std::wstring wcity;
	std::string city;
	std::string country_code;
	std::wstring wcountry;
	std::string country;
	std::string state_code {"none"};
	double lon;
	double lat;

	Location(std::string n, std::string c, std::string country_name, std::string s, double longitude, double latitude) 
		: city{ n }, country_code{ c }, country{ country_name },  state_code {s}, lon{ longitude }, lat{ latitude } {
		for (const auto& c : n) {
			wcity += static_cast<wchar_t>(c);
		}
		for (const auto& c : country_name) {
			wcountry += static_cast<wchar_t>(c);
		}
	}

	Location(std::string n, std::string c, double longitude, double latitude)
		: city{ n }, country_code{ c }, state_code{ "none" }, lon{ longitude }, lat{ latitude } {
		for (const auto& c : n) {
			wcity += static_cast<wchar_t>(c);
		}
	}

	Location(const Weather& w)
		: city{ w.city_input }, country_code{ w.country_code }, country{ w.country_name }, state_code{ w.state_code }, lon{ w.lon }, lat{ w.lat } {
		for (const auto& c : w.city_input) {
			wcity += static_cast<wchar_t>(c);
		}
		for (const auto& c : w.country_name) {
			wcountry += static_cast<wchar_t>(c);
		}
	}

	friend bool operator==(const Location& loc, const Weather& w) {
		return (loc.lat == w.lat) && (loc.lon == w.lon);
	}
};

class WorldSoundscape
{
private:
	bool paused {false};
	std::mutex push_back_mtx;
	std::shared_mutex display_shmtx;
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
	bool startMusic{false};
	bool exit_World_Soundscape{false};
	bool saved_locations_has_been_changed{false};
	WorldSoundscape();
	~WorldSoundscape();
	void setRandomLocation();
	void enterLocationMenu();
	void setUserlocation();
	void waitForCitiesListLoading();
	void mainMenu();
	void pauseMenu();
	void favouriteLocationsMenu();
	void infoMenu();
	void saveLocation();
	void loadSavedLocations();
	void saveChangesInSavedLocations();
	void displayWeather(Weather& weather, std::vector<std::string>& notes_played);
	void updateWeather(Weather& weather);
	void updateScale();
	std::vector<notes> getMode(Instrument&);
	void play_notes(Instrument& instrument, Weather& weather, std::vector<std::string>& notes_played);
	void initMusic();
	void keyboard_listener();
};

