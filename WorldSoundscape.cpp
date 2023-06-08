#include "WorldSoundscape.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include <random>
#include <limits>
#include <numeric>
#include <future>
#include <thread>
#include <iomanip>
#include "InstrumentInitializers.h"

#define KEY_R 82
#define KEY_r 114
#define KEY_N 78
#define KEY_n 110
#define KEY_E 69
#define KEY_e 101
#define KEY_U 85
#define KEY_u 117
#define KEY_M 77
#define KEY_m 109
#define KEY_Q 81
#define KEY_q 113
#define KEY_P 80
#define KEY_p 112
#define KEY_F 70
#define KEY_f 102

using namespace std::literals;
auto last_update_time = std::chrono::steady_clock::now();
auto last_display_time = std::chrono::steady_clock::now();

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

	Saved_Locations = {
		Location("Madrid", "ES", -3.7026, 40.4165),
		Location("London", "GB", -0.1257, 51.5085),
		Location("McMurdo Station", "AQ", 168.2227, -77.6554),
		Location("New York", "US", -74.006, 40.7143),
	};

	std::thread loadCitiesThread([this]() { weather.loadCitiesList(); });
	loadCitiesThread.detach();
	std::future<Instrument> jaguar_f = std::async(std::launch::async, CreateJaguarGuitar, reverbEffectSlot);
	std::future<Instrument> female_f = std::async(std::launch::async, CreateFemaleVoice, reverbEffectSlot);

	Jaguar = jaguar_f.get();
	FemaleVoice = female_f.get();
}

WorldSoundscape::~WorldSoundscape()
{
	alDeleteEffects(1, &reverbEffect);
	alDeleteAuxiliaryEffectSlots(1, &reverbEffectSlot);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void WorldSoundscape::waitForCitiesListLoading() {

	if (weather.cities.IsNull()) {
		std::cout << std::endl << std::setw(27) << "" << "[loading...]" << std::endl;
		while (weather.cities.IsNull())
			std::this_thread::sleep_for(100ms);
	}
}

void WorldSoundscape::mainMenu() {	
	bool valid_response{ false };
	int margin{ 20 };
	int option{ 0 };
	char key = ' ';
	while (key != 13 && !valid_response) {
		system("cls");
		std::cout << "\033[1;37m \n";
		std::cout << "         __    __    __   _______    ___    _ " << "  " << " ______\n";
		std::cout << "         \\ \\  /  \\  / /  / ______|  |   \\  | |" << "  " << "|  ___ \\\n";
		std::cout << "          \\ \\/ /\\ \\/ /   \\_____ \\   | |\\ \\ | |" << "  " << "| |   \\ \\\n";
		std::cout << "           \\  /  \\  /   ______/ /   | | \\ \\| |" << "  " << "| |___/ /\n";
		std::cout << "            \\/    \\/   |_______/    |_|  \\___|  |______/\n\n";
		std::cout << "\033[1;30m";
		std::cout << "           W O R L D   S O U N D  /  P R O C E D U R A L        \n";
		std::cout << "\033[0m";
		std::cout << "\n\n           [R] "; if (option == 0) std::cout << "\033[1;30;47m"; std::cout << "Random Location\n" << "\033[0m" <<
			"           [E] "; if (option == 1) std::cout << "\033[1;30;47m"; std::cout << "Enter Location\n" << "\033[0m" <<
			"           [U] "; if (option == 2) std::cout << "\033[1;30;47m"; std::cout << "User Location\n" << "\033[0m" <<
			"           [F] "; if (option == 3) std::cout << "\033[1;30;47m"; std::cout << "Favourite Locations\n" << "\033[0m" <<
			"           [Q] "; if (option == 4) std::cout << "\033[1;30;47m"; std::cout << "Quit\n" << "\033[0m";

		key = _getch();
		if (key == 0 || key == -32) {  // Arrow Keys first value
			key = _getch();

			switch (key) {
			case 80:
				option += 1;
				option = option > 4 ? 0 : option;
				break;
			case 72:
				option -= 1;
				option = option < 0 ? 3 : option;
				break;
			default:
				break;
			}
		}
		else {
			switch (key) {
			case KEY_R:
			case KEY_r:
				system("cls");
				std::cout << "\n";
				std::cout << std::left << std::setw(margin) << "" << "R A N D O M   L O C A T I O N" << std::endl;
				waitForCitiesListLoading();
				setRandomLocation();
				valid_response = true;
				break;

			case KEY_U:
			case KEY_u:
				system("cls");
				std::cout << "\n";
				std::cout << std::left << std::setw(margin+2) << "" << "U S E R   L O C A T I O N" << std::endl;
				setUserlocation();
				valid_response = true;
				break;

			case KEY_E:
			case KEY_e:
				system("cls");
				std::cout << "\n";
				std::cout << std::left << std::setw(margin+1) << "" << "E N T E R   L O C A T I O N" << std::endl;
				setUserEnterLocation();
				valid_response = true;
				break;

			case KEY_F:
			case KEY_f:
				system("cls");
				std::cout << "\n";
				std::cout << std::left << std::setw(margin -3) << "" << "F A V O U R I T E   L O C A T I O N S" << std::endl;
				favouriteLocationsMenu();
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
	if (!valid_response) {
		switch (option) {
		case 0:
			system("cls");
			std::cout << "\n";
			std::cout << std::left << std::setw(margin) << "" << "R A N D O M   L O C A T I O N" << std::endl;
			waitForCitiesListLoading();
			setRandomLocation();
			valid_response = true;
			break;

		case 1:
			system("cls");
			std::cout << "\n";
			std::cout << std::left << std::setw(margin + 1) << "" << "E N T E R   L O C A T I O N" << std::endl;
			setUserEnterLocation();
			valid_response = true;
			break;

		case 2:
			system("cls");
			std::cout << "\n";
			std::cout << std::left << std::setw(margin + 2) << "" << "U S E R   L O C A T I O N" << std::endl;
			setUserlocation();
			valid_response = true;
			break;

		case 3:
			system("cls");
			std::cout << "\n";
			std::cout << std::left << std::setw(margin +1) << "" << "S A V E D   L O C A T I O N S" << std::endl;
			favouriteLocationsMenu();
			valid_response = true;
			break;

		case 4:
			system("cls");
			std::cout << "Exiting programm..." << std::endl;
			stop_flag = true;
			cv.notify_all();
			exit_World_Soundscape = true;
			valid_response = true;
			break;

		default:			
			std::cout << "Error in option switch main menu" << std::endl;
			break;
		}
	}
}

void WorldSoundscape::favouriteLocationsMenu(){
	int capacity{ 50 };
	int space(19);
	int total_space(61);
	int option = 0;
	char key = ' ';
	int margin{ 4 };

	while (key != 13 || option >= Saved_Locations.size()) {
		margin = 4;
		system("cls");
		std::cout << std::endl;
		std::cout << std::setw(margin + 3) << "" << std::setw(space) << "City" << std::setw(space) << "Country" << std::setw(space / 1.7) << "Longitude" << "Latitude\n\n";
		for (int i{ 0 }; i < Saved_Locations.size(); ++i) {
			if (i > 9) margin = 3;			
			std::cout << std::setw(margin) << "" << i + 1 << "[ ";
			if (option == i) std::cout << "\033[1;30;47m";
			std::cout << std::setw(space) << Saved_Locations.at(i).city << std::setw(space) << Saved_Locations.at(i).country_code << std::setw(space / 1.7) << Saved_Locations.at(i).lon << std::setw(9) << Saved_Locations.at(i).lat;
			std::cout << "\033[0m" << " ]\n";
			std::cout << "\033[0m";
		}
		for (int i{ 0 }; i < (capacity - Saved_Locations.size()); ++i) {
			if ((i + Saved_Locations.size() + 1) > 9) margin = 3;			
			std::cout << std::setw(margin) << "" << (i + Saved_Locations.size()) + 1; 
			if (option == i + Saved_Locations.size()) std::cout << "\033[1;30;47m";
			std::cout << std::setw(total_space) << "[ " << "]\n";
			std::cout << "\033[0m";
		}

		key = _getch();
		if (key == 0 || key == -32) {  // Arrow Keys first value
			key = _getch();

			switch (key) {
			case 80:
				option += 1;
				option = option > capacity ? 0 : option;
				break;
			case 72:
				option -= 1;
				option = option < 0 ? capacity : option;
				break;
			default:
				break;
			}
		}
		if (key == 13 && option >= Saved_Locations.size()) { 
			std::cout << "\033[1;31m \n";
			std::cout << "   ERROR: empty block";
			std::cout << "\033[0m";
			std::this_thread::sleep_for(1s);
		}
	}

	std::cout << std::endl << std::setw(27) << "" << "[loading...]" << std::endl;
	weather.city_input = Saved_Locations.at(option).city;
	weather.city = weather.city_input;
	weather.whiteSpaceURLManager(weather.city);
	weather.country_code = Saved_Locations.at(option).country_code;
	if (Saved_Locations.at(option).state_code != "none") {
		weather.state_code = Saved_Locations.at(option).state_code;
	}
	weather.callAllAPIs();
	updateScale();
}

void WorldSoundscape::initMusic() {
	std::vector<std::string > notes_played;
	std::jthread update_weather_thread(&WorldSoundscape::updateWeather, this, std::ref(weather));
	std::jthread display_weather_thread(&WorldSoundscape::displayWeather, this, std::ref(weather), std::ref(notes_played));
	std::jthread jaguar_guitar_thread(&WorldSoundscape::play_notes, this, std::ref(Jaguar), std::ref(weather), std::ref(notes_played));
	std::jthread female_voice_thread(&WorldSoundscape::play_notes, this, std::ref(FemaleVoice), std::ref(weather), std::ref(notes_played));
	std::jthread keyboard_listener_thread(&WorldSoundscape::keyboard_listener, this);
}

void WorldSoundscape::setUserEnterLocation(){
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
		std::cout << "Enter the state code for more accuracy if needed or press enter: ";
		std::getline(std::cin, weather.state_code);
		if (weather.state_code.size() == 0) weather.state_code = "none";
	}
	else {
		weather.state_code = "none";
	}
	weather.user_location = false;
	weather.callAllAPIs();
	updateScale();
	last_update_time = std::chrono::steady_clock::now();
	update_mtx.unlock();
}

void WorldSoundscape::setUserlocation(){
	update_mtx.lock();
	cv.notify_all();
	weather.user_location = true;
	weather.callAllAPIs();
	updateScale();
	last_update_time = std::chrono::steady_clock::now();
	update_mtx.unlock();
}

void WorldSoundscape::setRandomLocation() {
	if (!weather.cities.IsNull()) {	
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, weather.cities.Size() - 1);
		int index = dis(gen);
		const rapidjson::Value& city = weather.cities[index];
		const char* city_name = city["name"].GetString();
		const char* country_code = city["country"].GetString();
		update_mtx.lock();
		cv.notify_all();
		weather.city = city_name;
		weather.city_input = city_name;
		weather.whiteSpaceURLManager(weather.city);
		weather.country_code = country_code;
		weather.user_location = false;
		weather.callAllAPIs();
		updateScale();
		last_update_time = std::chrono::steady_clock::now();
		update_mtx.unlock();
	}
	else { std::cerr << "cities list wasn't ready for random location";}
}

void WorldSoundscape::displayWeather(Weather& weather, std::vector<std::string>& notes_played) {
	std::mutex mtx;
	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		std::shared_lock update_lock(update_mtx);
		while (std::chrono::steady_clock::now() < (last_display_time + 900ms)) {
			std::this_thread::sleep_for(10ms);
		}
		shmtx.lock(); 
		system("CLS");
		weather.display();
		std::cout << "\n";
		for (auto& n : notes_played)
			std::cout << n << " ";
		last_display_time = std::chrono::steady_clock::now();
		shmtx.unlock();
		update_lock.unlock();
		cv.wait_for(lock, 100ms);
	}
}

void WorldSoundscape::updateWeather(Weather& weather) {
	std::mutex mtx;
	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		float reverbGain = static_cast<float>(weather.humidity) / 100.0f;
		alEffectf(reverbEffect, AL_EAXREVERB_GAIN, reverbGain);
		alAuxiliaryEffectSloti(reverbEffectSlot, AL_EFFECTSLOT_EFFECT, reverbEffect);
		while (std::chrono::steady_clock::now() < (last_update_time + 5s)) {
			std::this_thread::sleep_for(100ms);
		}
		if (!stop_flag) {
			std::shared_lock up_lock(update_mtx);
			weather.callAllAPIs();
			updateScale();
			last_update_time = std::chrono::steady_clock::now();
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
		if (notes_played.size() >= 20){
			shmtx.lock();
			try {
				notes_played.clear();
			}
			catch (std::exception& e) { std::cerr << "exception caught: " << e.what() << std::endl;}
			shmtx.unlock();
		}
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

void WorldSoundscape::updateScale(){
	guitar_mode = setScaleForInstrument(Jaguar, weather);
	female_voice_mode = setScaleForInstrument(FemaleVoice, weather);
}

std::vector<notes> WorldSoundscape::getMode(Instrument& i){
	if (i.min == -1) return female_voice_mode;
	else if (i.min == -22) return guitar_mode;
	else {
		std::cout << "no mode found in get mode function\n";
	}
}