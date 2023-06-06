#include "WorldSoundscape.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include <random>
#include <limits>
#include <numeric>
using namespace std::literals;

WorldSoundscape::WorldSoundscape() : weather{ false, "none", "none", "none" } {

	alcMakeContextCurrent(context);
	alGenEffects(1, &reverbEffect);
	alEffecti(reverbEffect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
	alGenAuxiliaryEffectSlots(1, &reverbEffectSlot);
	alEffectf(reverbEffect, AL_EAXREVERB_DENSITY, 1.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_DIFFUSION, 1.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_GAIN, 0.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_GAINHF, 0.1f);
	alEffectf(reverbEffect, AL_EAXREVERB_GAINLF, 0.9f);
	alEffectf(reverbEffect, AL_EAXREVERB_DECAY_TIME, 15.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_DECAY_HFRATIO, 0.7f);
	alEffectf(reverbEffect, AL_EAXREVERB_DECAY_LFRATIO, 0.6f);
	alEffectf(reverbEffect, AL_EAXREVERB_REFLECTIONS_GAIN, 0.1f);
	alEffectf(reverbEffect, AL_EAXREVERB_REFLECTIONS_DELAY, 0.05f);
	alEffectf(reverbEffect, AL_EAXREVERB_LATE_REVERB_GAIN, 1.1f);
	alEffectf(reverbEffect, AL_EAXREVERB_LATE_REVERB_DELAY, 0.002f);
	alEffectf(reverbEffect, AL_EAXREVERB_ECHO_TIME, 0.25f);
	alEffectf(reverbEffect, AL_EAXREVERB_ECHO_DEPTH, 0.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_MODULATION_TIME, 3.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_MODULATION_DEPTH, 0.1f);
	alEffectf(reverbEffect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, 1.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, 0.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_HFREFERENCE, 2000.0f);
	alEffectf(reverbEffect, AL_EAXREVERB_LFREFERENCE, 140.0f);

	alAuxiliaryEffectSloti(reverbEffectSlot, AL_EFFECTSLOT_EFFECT, reverbEffect);

	Jaguar = CreateJaguarGuitar(reverbEffectSlot);
	FemaleVoice = CreateFemaleVoice(reverbEffectSlot);
}

WorldSoundscape::~WorldSoundscape()
{
	alDeleteEffects(1, &reverbEffect);
	alDeleteAuxiliaryEffectSlots(1, &reverbEffectSlot);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void WorldSoundscape::mainMenu() {
	std::cout << "Welcome to World Soundscape, press the following keys to select an option:\n" <<
		"R: Random Location\n" <<
		"E: Enter Location\n" <<
		"U: User Location\n" <<
		"Q: Exit\n";

	bool valid_response{ false };
	while (!valid_response) {
		switch (_getch()) {
		case KEY_R:
		case KEY_r:
			system("cls");
			std::cout << "R A N D O M   L O C A T I O N" << std::endl;
			setRandomLocation();
			valid_response = true;
			break;

		case KEY_U:
		case KEY_u:
			system("cls");
			std::cout << "U S E R   L O C A T I O N" << std::endl;
			setUserlocation();
			valid_response = true;
			break;

		case KEY_E:
		case KEY_e:
			system("cls");
			std::cout << "E N T E R   L O C A T I O N" << std::endl;
			setUserEnterLocation();
			valid_response = true;
			break;

		case KEY_Q:
		case KEY_q:
			system("cls");
			std::cout << "Exiting programm..." << std::endl;
			stop_flag = true;
			cv.notify_all();
			exit_World_Soundscape = true;
			valid_response = true;
			break;

		default:
			std::cout << "Wrong command, try again..." << std::endl;
		}
	}
}

void WorldSoundscape::initMusic() {
	//Instrument Jaguar = CreateJaguarGuitar(reverbEffectSlot);
	//Instrument FemaleVoice = CreateFemaleVoice(reverbEffectSlot);
	std::vector<std::string > notes_played;
	std::jthread update_weather_thread(&WorldSoundscape::updateWeather, this, std::ref(weather));
	std::jthread display_weather_thread(&WorldSoundscape::displayWeather, this, std::ref(weather), std::ref(notes_played));
	std::jthread jaguar_guitar_thread(&WorldSoundscape::play_notes, this, std::ref(Jaguar), std::ref(weather), std::ref(notes_played));
	std::jthread female_voice_thread(&WorldSoundscape::play_notes, this, std::ref(FemaleVoice), std::ref(weather), std::ref(notes_played));
	std::jthread keyboard_listener_thread(&WorldSoundscape::keyboard_listener, this);
}

void WorldSoundscape::setUserEnterLocation()
{
	update_mtx.lock();
	system("cls");
	std::cout << "New User Location..." << std::endl;
	std::cout << "\nPlease enter the city name: ";
	std::getline(std::cin, weather.city_input);
	weather.city = weather.city_input;
	weather.whiteSpaceURLManager(weather.city);
	std::cout << "Please enter the country code: ";
	std::getline(std::cin, weather.country_code);

	if (weather.country_code == "US") {
		std::cout << "Please enter the state code for more accuracy: ";
		std::getline(std::cin, weather.state_code);
	}
	else {
		weather.state_code = "none";
	}
	weather.user_location = false;
	weather.callAllAPIs();
	updateScale();
	update_mtx.unlock();
}

void WorldSoundscape::setUserlocation()
{
	update_mtx.lock();
	cv.notify_all();
	weather.user_location = true;
	weather.callAllAPIs();
	updateScale();
	update_mtx.unlock();
}

void WorldSoundscape::setRandomLocation() {
	if (weather.cities.IsNull()) {
		std::ifstream file("cities.json");
		std::string str((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		weather.cities.Parse(str.c_str());
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, weather.cities.Size() - 1);
	int index = dis(gen);
	const rapidjson::Value& city = weather.cities[index];
	const char* city_name = city["name"].GetString();
	const char* country_code = city["country"].GetString();
	update_mtx.lock();
	cv.notify_all();
	//system("cls");
	//std::cout << "New Random Location..." << std::endl;
	weather.city = city_name;
	weather.city_input = city_name;
	weather.whiteSpaceURLManager(weather.city);
	weather.country_code = country_code;
	weather.user_location = false;
	weather.callAllAPIs();
	updateScale();
	update_mtx.unlock();
}

void WorldSoundscape::displayWeather(Weather& weather, std::vector<std::string>& notes_played) {
	std::mutex mtx;
	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		std::shared_lock update_lock(update_mtx);
		shmtx.lock(); 
		system("CLS");
		weather.display();
		std::cout << "\n";
		for (auto& n : notes_played)
			std::cout << n << " ";

		shmtx.unlock();
		update_lock.unlock();
		cv.wait_for(lock, 1s);
	}
}

void WorldSoundscape::updateWeather(Weather& weather) {
	std::mutex mtx;
	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		float reverbGain = static_cast<float>(weather.humidity) / 100.0f;
		alEffectf(reverbEffect, AL_EAXREVERB_GAIN, reverbGain);
		alAuxiliaryEffectSloti(reverbEffectSlot, AL_EFFECTSLOT_EFFECT, reverbEffect);
		std::this_thread::sleep_for(5s);
		if (!stop_flag) {
			std::shared_lock up_lock(update_mtx);
			weather.callAllAPIs();
			updateScale();
			up_lock.unlock();
		}
	}
}

void WorldSoundscape::play_notes(Instrument& instrument, Weather& weather, std::vector<std::string>& notes_played) {
	srand(time(0));
	std::this_thread::sleep_for(1s);
	std::vector<notes> mode = getMode(instrument);
	int r = rand() % mode.size();
	std::mutex mtx;

	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		if (notes_played.size() >= 20)
			notes_played.clear();
		std::shared_lock update_lock(update_mtx);
		mode = getMode(instrument);
		r = rand() % mode.size();
		instrument.sounds[mode[r]].play();
		std::shared_lock sh_lock(shmtx);
		std::cout << instrument.sounds[mode[r]].sharp_name << " ";
		notes_played.push_back(instrument.sounds[mode[r]].sharp_name);
		sh_lock.unlock();
		update_lock.unlock();

		int sleep_time;
		if (weather.wind_speed != 0) {
			sleep_time = static_cast<int>((rand() % instrument.sleep_value) / (weather.wind_speed / 1.7));
		}
		else {
			sleep_time = static_cast<int>((rand() % instrument.sleep_value) + 1000);
		}
		cv.wait_for(lock, std::chrono::milliseconds(sleep_time));
	}
}

void WorldSoundscape::keyboard_listener() {

	int value{ 0 };
	while (value != KEY_Q) {
		switch (_getch()) {
		case KEY_R:
		case KEY_r:
			setRandomLocation();
			break;

		case KEY_E:
		case KEY_e:
			setUserEnterLocation();
			break;

		case KEY_M:
		case KEY_m:
			system("cls");
			std::cout << "Back to menu..." << std::endl;
			cv.notify_all();
			stop_flag = true;
			break;
		case KEY_U:
		case KEY_u:
			setUserlocation();
			break;
		case KEY_Q:
		case KEY_q:
			update_mtx.lock();
			system("cls");
			std::cout << "Exiting programm..." << std::endl;
			stop_flag = true;
			cv.notify_all();
			exit_World_Soundscape = true;
			value = KEY_Q;
			update_mtx.unlock();
			break;

		default:
			std::cout << " Wrong command" << std::endl;
		}
	}
}

void WorldSoundscape::updateScale()
{
	guitar_mode = setScaleForInstrument(Jaguar, weather);
	female_voice_mode = setScaleForInstrument(FemaleVoice, weather);
}

std::vector<notes> WorldSoundscape::getMode(Instrument& i)
{
	if (i.min == -1) return female_voice_mode;
	else if (i.min == -22) return guitar_mode;
	else {
		std::cout << "no mode found in get mode function\n";
	}
}