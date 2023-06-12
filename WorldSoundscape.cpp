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

#define KEY_B 66
#define KEY_b 98
#define KEY_D 68
#define KEY_d 100
#define KEY_E 69
#define KEY_e 101
#define KEY_F 70
#define KEY_f 102
#define KEY_I 73
#define KEY_i 105
#define KEY_L 76
#define KEY_l 108
#define KEY_M 77
#define KEY_m 109
#define KEY_N 78
#define KEY_n 110
#define KEY_P 80
#define KEY_p 112
#define KEY_Q 81
#define KEY_q 113
#define KEY_R 82
#define KEY_r 114
#define KEY_s 115
#define KEY_S 83
#define KEY_U 85
#define KEY_u 117
#define KEY_Y 89
#define KEY_y 121

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
	system("chcp 65001");
	system("cls");
	loadSavedLocations();
	std::thread loadCitiesThread([this]() { weather.loadCitiesList(); });
	loadCitiesThread.detach();
	std::future<Instrument> jaguar_f = std::async(std::launch::async, CreateJaguarGuitar, reverbEffectSlot);
	std::future<Instrument> female_f = std::async(std::launch::async, CreateFemaleVoice, reverbEffectSlot);

	Jaguar = jaguar_f.get();
	FemaleVoice = female_f.get();
}

WorldSoundscape::~WorldSoundscape()
{
	if (saved_locations_has_been_changed) {
		saveChangesInSavedLocations();
	}
	alDeleteEffects(1, &reverbEffect);
	alDeleteAuxiliaryEffectSlots(1, &reverbEffectSlot);
	alcDestroyContext(context);
	alcCloseDevice(device);

	std::cout << "         Thank you for using World Sound :)\n";
	std::this_thread::sleep_for(1s);
}

void WorldSoundscape::loadSavedLocations() {
	
	std::ifstream file("saved_locations.txt");
	if (!file) {
		std::cerr << "saved_locations.txt not found";
		weather.No_Saved_Locations = true;
	}
	else {
		weather.No_Saved_Locations = false;
		std::string line;

		auto lambdaget = [](std::string& line) {
			auto it = std::find(line.begin(), line.end(), '/');
			std::string result(line.begin(), it);
			line.erase(line.begin(), it + 1);
			return result;
		};
		while (std::getline(file, line)) {

			std::string city = lambdaget(line);
			std::string country_code = lambdaget(line);
			std::string country = lambdaget(line);
			std::string state_code = lambdaget(line);
			std::string temp = lambdaget(line);
			double lon = std::stod(temp);
			std::string temp2(line.begin(), line.end());
			double lat = std::stod(temp2);

			Saved_Locations.emplace_back(Location(city, country_code, country, state_code, lon, lat));
		}
	}

}

void WorldSoundscape::saveChangesInSavedLocations() {
	
	int margin{ 9 };
	system("cls");
	std::cout << std::endl;
	std::cout << std::setw(margin) << "" << "Saved Locations has been changed, do you want to save?\n\n";
	std::cout << std::setw(margin * 2) << "" << std::setw(margin * 3) << "[N] No"  << "[Y] Yes\n\n";
	char key = ' ';
	bool save_locations{ false };
	while (key != KEY_N && key != KEY_n && key != KEY_Y && key != KEY_y) {
		key = _getch();
		switch (key) {
		case KEY_Y:
		case KEY_y:
			save_locations = true;
			break;
		case KEY_N:
		case KEY_n:
			break;
		}
	}
	if (save_locations) {
		std::ofstream file("saved_locations.txt");
		if (file.is_open()) {
			for (auto l : Saved_Locations) {
				file << l.city << "/" << l.country_code << "/" << l.country << "/" << l.state_code << "/" << l.lon << "/" << l.lat << std::endl;
			}
			file.close();
			std::cout << std::setw(margin) << "" << "Locations saved successfully!" << std::endl;
		}
		else {
			std::cout << std::setw(margin) << "" << "Failed to save saved_locations.txt" << std::endl;
		}
	}
}

void WorldSoundscape::waitForCitiesListLoading() {

	if (weather.cities.IsNull()) {
		std::cout << std::endl << std::setw(27) << "" << "loading [...]" << std::endl;
		while (weather.cities.IsNull())
			std::this_thread::sleep_for(100ms);
	}
}

void WorldSoundscape::mainMenu() {	
	bool out_of_loop{false};
	int margin{20};
	int space{10};
	int option{0};
	char key = ' ';
	while (key != 13 && !out_of_loop) {
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
			"           [S] "; if (option == 3) std::cout << "\033[1;30;47m"; std::cout << "Saved Locations\n" << "\033[0m" <<
			"           [I] "; if (option == 4) std::cout << "\033[1;30;47m"; std::cout << "Info\n" << "\033[0m" <<
			"           [Q] "; if (option == 5) std::cout << "\033[1;30;47m"; std::cout << "Quit\n" << "\033[0m";
		
		std::cout << std::endl;
		if (weather.No_Conexion) std::cout << std::setw(space) << "" << "\033[1;31m" << "Error: couldn't establish connection\n" << "\033[0m";
		if (weather.No_ApiCode) std::cout << std::setw(space) << "" << "\033[1;31m" << "Error: couldn't find apicode.txt file\n" << "\033[0m";
		if (weather.No_Saved_Locations) std::cout << std::setw(space) << "" << "\033[1;31m" << "Error: couldn't find saved_locations.txt file\n" << "\033[0m";

		key = _getch();
		if (key == 0 || key == -32) {  // Arrow Keys first value
			key = _getch();

			switch (key) {
			case 80:
				option += 1;
				option = option > 5 ? 0 : option;
				break;
			case 72:
				option -= 1;
				option = option < 0 ? 5 : option;
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
				out_of_loop = true;
				break;

			case KEY_U:
			case KEY_u:
				system("cls");
				std::cout << "\n";
				std::cout << std::left << std::setw(margin+2) << "" << "U S E R   L O C A T I O N" << std::endl;
				setUserlocation();
				out_of_loop = true;
				break;

			case KEY_E:
			case KEY_e:
				system("cls");
				std::cout << "\n";
				std::cout << std::left << std::setw(margin+1) << "" << "E N T E R   L O C A T I O N" << std::endl;
				enterLocationMenu();
				out_of_loop = true;
				break;

			case KEY_S:
			case KEY_s:
				system("cls");
				std::cout << "\n";
				std::cout << std::left << std::setw(margin +1) << "" << "S A V E D   L O C A T I O N S" << std::endl;
				favouriteLocationsMenu();
				out_of_loop = true;
				break;

			case KEY_I:
			case KEY_i:
				system("cls");
				std::cout << "\n";
				infoMenu();
				startMusic = false;
				out_of_loop = true;
				break;

			case KEY_Q:
			case KEY_q:
				system("cls");
				std::cout << "Exiting programm..." << std::endl;
				stop_flag = true;
				cv.notify_all();
				exit_World_Soundscape = true;
				out_of_loop = true;
				startMusic = false;
				break;

			default:
				std::cout << "Wrong command, try again..." << std::endl;
			}
		}
	}
	if (!out_of_loop) {
		switch (option) {
		case 0:
			system("cls");
			std::cout << "\n";
			std::cout << std::left << std::setw(margin) << "" << "R A N D O M   L O C A T I O N" << std::endl;
			waitForCitiesListLoading();
			setRandomLocation();
			out_of_loop = true;
			break;

		case 1:
			system("cls");
			std::cout << "\n";
			std::cout << std::left << std::setw(margin + 1) << "" << "E N T E R   L O C A T I O N" << std::endl;
			enterLocationMenu();
			out_of_loop = true;
			break;

		case 2:
			system("cls");
			std::cout << "\n";
			std::cout << std::left << std::setw(margin + 2) << "" << "U S E R   L O C A T I O N" << std::endl;
			setUserlocation();
			out_of_loop = true;
			break;

		case 3:
			system("cls");
			std::cout << "\n";
			std::cout << std::left << std::setw(margin +1) << "" << "S A V E D   L O C A T I O N S" << std::endl;
			favouriteLocationsMenu();
			out_of_loop = true;
			break;

		case 4:
			system("cls");
			std::cout << "\n";
			infoMenu();
			startMusic = false;
			out_of_loop = true;
			break;

		case 5:
			system("cls");
			std::cout << "Exiting programm..." << std::endl;
			stop_flag = true;
			cv.notify_all();
			exit_World_Soundscape = true;
			out_of_loop = true;
			break;

		default:			
			std::cout << "Error in option switch main menu" << std::endl;
			break;
		}
	}
}

std::string reduceString(std::string s, int n) {
	std::string result;
	for (int i{ 0 }; i < n; ++i) {
		result += s.at(i);
	}
	result += "...";
	return result;
}

std::wstring reduceString(std::wstring s, int n) {
	std::wstring result;
	for (int i{ 0 }; i < n; ++i) {
		result += s.at(i);
	}
	wchar_t point{ '.' };
	int p{ 3 };
	while (p--) {
	result += point;
	}
	return result;
}

void WorldSoundscape::pauseMenu() {
	update_mtx.lock();
	std::cout << "\nPaused, press \"P\" to continue...";
	char key = ' ';
	while (key != KEY_P && key != KEY_p) {
		key = _getch();
	}
	update_mtx.unlock();
}

int dif(const std::string& s) {
	size_t len{ 0 };
	for (int i{ 0 }; i < s.size(); ++i) {
		len += 1;
		if ((s[i] == 'Ã' || s[i] == 'Å' || s[i] == '€' || s[i] == 'â' || s[i] == 'Ä') && i<15) len -= 1;
	}
	return s.size() - len;
}

void WorldSoundscape::favouriteLocationsMenu(){
	int capacity{ 25 };
	int space(19);
	int total_space(60);
	int option = 0;
	char key = ' ';
	int margin{ 4 };
	bool out{false};
	bool back{ false };

	update_mtx.lock();
	cv.notify_all();

	while (!out) {
		margin = 4;
		system("cls");
		std::cout << std::endl;
		std::cout << std::setw(margin + 3) << "" << std::setw(space) << "City" << std::setw(space) << "Country" << std::setw(space / 1.7) << "Longitude" << "Latitude\n\n";
		for (int i{ 0 }; i < Saved_Locations.size(); ++i) {
			if (i > 8) margin = 3;			
			std::cout << std::setw(margin) << "" << i + 1 << "[ ";
			if (option == i) std::cout << "\033[1;30;47m";			
			std::string city = Saved_Locations.at(i).city;
			if ((city.size() - dif(city)) < 18) {
				std::cout << std::setw(space + dif(city));
				std::cout << city;
			}
			else { 
				std::string result = reduceString(city, 15+dif(city));
				std::cout << std::setw(space + dif(city));
				std::cout << result; 
			}
			std::string country = Saved_Locations.at(i).country;

			if ((country.size() - dif(country)) < 18) {
				std::cout << std::setw(space + dif(country));
				std::cout << country;
			}
			else { 
				std::string result = reduceString(country, 15 + dif(country));
				std::cout << std::setw(space + dif(country));
				std::cout << result;
			}
			std::cout << std::setw(space / 1.7) << Saved_Locations.at(i).lon << std::setw(8) << Saved_Locations.at(i).lat;
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
		std::cout << std::endl;
		std::cout << std::setw(margin) << "" << std::setw(total_space-18) << "[D] Delete Location" << "[ENTER] Load Location" << std::endl;
		std::cout << std::setw(margin) << "" << std::setw(total_space / 2) << "[B] Back" << std::endl;

		key = _getch();
		if (key == 0 || key == -32) {  // Arrow Keys first value
			key = _getch();

			switch (key) {
			case 80:
				option += 1;
				option = option > (capacity -1) ? 0 : option;
				break;
			case 72:
				option -= 1;
				option = option < 0 ? (capacity - 1) : option;
				break;
			default:
				break;
			}
		}
		else if (key == 13 && option >= Saved_Locations.size()) {
			std::cout << "\033[1;31m \n";
			std::cout << "   ERROR: empty block";
			std::cout << "\033[0m";
			std::this_thread::sleep_for(400ms);
		}
		else if (key == 13) {
			out = true;
			startMusic = true;
			stop_flag = false;
		}
		else {
			switch (key) {
			case KEY_B:
			case KEY_b:
				out = true;
				back = true;
				break;
			case KEY_D:
			case KEY_d:
				if (option >= Saved_Locations.size()) {
					std::cout << "\033[1;31m \n";
					std::cout << "   ERROR: empty block";
					std::cout << "\033[0m";
					std::this_thread::sleep_for(400ms);
				}
				else {
					saved_locations_has_been_changed = true;
					Saved_Locations.erase(Saved_Locations.begin() + option);
				}
				break;
			}
		}
	}

	if (!back) {
		std::cout << std::endl << std::setw(27) << "" << "loading [...]" << std::endl;
		weather.city_input = Saved_Locations.at(option).city;
		weather.city = weather.city_input;
		weather.whiteSpaceURLManager(weather.city);
		weather.country_code = Saved_Locations.at(option).country_code;
		weather.state_code = Saved_Locations.at(option).state_code;
		weather.user_location = false;
		weather.callAllAPIs();
		updateScale();
	}
	update_mtx.unlock();
}

void WorldSoundscape::infoMenu(){
	int margin(8);
	int space(56);
	std::cout << "\n\n";
	//std::cout << std::setfill(' ') << std::setw(margin) << "" << std::setfill('-') << std::setw(space) << "" << std::endl;
	std::cout << std::setfill(' ') << std::setw(margin) << "" << "World Sound is a procedural and musical application \n"; 
	std::cout << std::setw(margin) << "" << "developed by Jorge Manzano, with the precious help of\n";
	std::cout << std::setw(margin) << "" << "his beloved musician friends. The application retrieves\n";
	std::cout << std::setw(margin) << "" << "weather data from openweathermap.org/api, sunrise and \n";
	std::cout << std::setw(margin) << "" << "sunset data from sunrise-sunset.org/api, and moon data\n";
	std::cout << std::setw(margin) << "" << "from aa.usno.navy.mil/data/api. The data gets procesed\n";
	std::cout << std::setw(margin) << "" << "in real time into a musical form that changes with each\n";
	std::cout << std::setw(margin) << "" << "weather and astronomic update. In order to enjoy World\n";
	std::cout << std::setw(margin) << "" << "Sound, you'll need to registrer freely in OpenWeatherMap\n";
	std::cout << std::setw(margin) << "" << "and create a txt file named \"apicode.txt\" in the app's\n";
	std::cout << std::setw(margin) << "" << "folder with the API code.\n\n";
	std::cout << std::setw(margin) << "" << "Thanks for using World Sound!\n";
	std::cout << std::setw(space-10) << "" << "June 10th 2023\n\n";
	std::cout << std::setw(margin) << "" << "[B] Back\n";

	char key = ' ';
	while (key != KEY_B && key != KEY_b) {
		key = _getch();
	}
}

void WorldSoundscape::saveLocation(){

	bool found{ false };
	for (auto l : Saved_Locations) {
		if (l == weather)
			found = true;
	}
	if (!found && Saved_Locations.size() < 25) {
		Location new_location = weather;
		Saved_Locations.push_back(new_location);
		saved_locations_has_been_changed = true;
	}
}

void WorldSoundscape::initMusic() {
	std::vector<std::string > notes_played;
	std::jthread update_weather_thread(&WorldSoundscape::updateWeather, this, std::ref(weather));
	std::jthread display_weather_thread(&WorldSoundscape::displayWeather, this, std::ref(weather), std::ref(notes_played));
	std::jthread jaguar_guitar_thread(&WorldSoundscape::play_notes, this, std::ref(Jaguar), std::ref(weather), std::ref(notes_played));
	std::jthread female_voice_thread(&WorldSoundscape::play_notes, this, std::ref(FemaleVoice), std::ref(weather), std::ref(notes_played));
	std::jthread keyboard_listener_thread(&WorldSoundscape::keyboard_listener, this);
}

void WorldSoundscape::enterLocationMenu(){
	update_mtx.lock();
	cv.notify_all();
	stop_flag = false;
	system("cls");
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
	startMusic = true;
	update_mtx.unlock();
}

void WorldSoundscape::setUserlocation(){
	update_mtx.lock();
	stop_flag = false;
	cv.notify_all();
	weather.user_location = true;
	weather.callAllAPIs();
	updateScale();
	last_update_time = std::chrono::steady_clock::now();
	startMusic = true;
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
		startMusic = true;
		stop_flag = false;
		update_mtx.unlock();
	}
	else { std::cerr << "cities list wasn't ready for random location";}
}

void WorldSoundscape::displayWeather(Weather& weather, std::vector<std::string>& notes_played) {
	std::mutex mtx;
	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		std::shared_lock update_lock(update_mtx);
		while (std::chrono::steady_clock::now() < (last_display_time + 900ms) && !stop_flag) {
			std::this_thread::sleep_for(10ms);
		}		
		if (!stop_flag) {
			display_shmtx.lock();
			system("CLS");
			weather.display();
			std::cout << "\n";
			try {
				for (auto& n : notes_played)
					std::cout << n << " ";
			}
			catch (...) {
				system("cls");
				std::cerr << "Exception in display weather thread.\n";
				std::this_thread::sleep_for(3600s);
			}
			display_shmtx.unlock();
			last_display_time = std::chrono::steady_clock::now();
		}		
		update_lock.unlock();
		cv.wait_for(lock, 100ms);
	}
}

void WorldSoundscape::updateWeather(Weather& weather) {
	while (!stop_flag) {
		float reverbGain = static_cast<float>(weather.humidity) / 100.0f;
		alEffectf(reverbEffect, AL_EAXREVERB_GAIN, reverbGain);
		alAuxiliaryEffectSloti(reverbEffectSlot, AL_EFFECTSLOT_EFFECT, reverbEffect);
		while (std::chrono::steady_clock::now() < (last_update_time + 5s) && !stop_flag) {
			std::this_thread::sleep_for(100ms);
		}	
		if (!stop_flag) {	
			std::shared_lock up_lock(update_mtx);
			//std::cout << "[Update start] ";
			weather.callAllAPIs();
			updateScale();
			//std::cout << "[Update finish] ";
			up_lock.unlock();
			last_update_time = std::chrono::steady_clock::now();			
		}		
	}
}

int rnd_gen(int min, int max){
	std::random_device rd;
	std::mt19937 gen {rd()};
	std::uniform_int_distribution d(min, max);

	return d(gen);
}

void WorldSoundscape::play_notes(Instrument& instrument, Weather& weather, std::vector<std::string>& notes_played) {
	srand(time(0));
	std::vector<notes> mode = getMode(instrument);
	//int r = rand() % mode.size();
	std::random_device rd;
	std::mt19937 gen {rd()};
	std::uniform_int_distribution d(0, 10000);
	int r = d(gen) % mode.size();
	std::mutex mtx;

	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		
		if (notes_played.size() >= 20){			
			try {
				display_shmtx.lock();
				notes_played.clear();
				display_shmtx.unlock();
			}
			catch (...) { 
				std::cerr << "exception in clear vector.\n";
				std::this_thread::sleep_for(3600s);
			}			
		}		
		std::shared_lock update_lock(update_mtx);
		mode = getMode(instrument);
		//r = rand() % mode.size();
		r = d(gen) % mode.size();
		instrument.sounds[mode[r]].play();

		std::shared_lock display_shlock(display_shmtx);
		if (!stop_flag)
		std::cout << instrument.sounds[mode[r]].sharp_name << " ";
		try {
			push_back_mtx.lock();
			notes_played.push_back(instrument.sounds[mode[r]].sharp_name);
			push_back_mtx.unlock();
		} catch (...) { 
			system("cls");
			std::cerr << "exception in play notes thread.\n";
			std::this_thread::sleep_for(3600s);
		}
		display_shlock.unlock();
		update_lock.unlock();
		

		int sleep_time;
		if (weather.wind_speed != 0) {
			//sleep_time = static_cast<int>((rand() % instrument.sleep_value) / (weather.wind_speed / 1.7));
			sleep_time = static_cast<int>((d(gen) % instrument.sleep_value) / (weather.wind_speed / 1.7));
		}
		else {
			//sleep_time = static_cast<int>((rand() % instrument.sleep_value) + 1000);
			sleep_time = static_cast<int>((d(gen) % instrument.sleep_value) + 1000);
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
			enterLocationMenu();
			break;
		case KEY_M:
		case KEY_m:			
			update_mtx.lock();
			system("cls");
			std::cout << std::endl << std::setw(27) << "" << "loading [...]" << std::endl;
			cv.notify_all();
			stop_flag = true;
			startMusic = false;
			value = KEY_Q;
			update_mtx.unlock();
			break;
		case KEY_U:
		case KEY_u:
			setUserlocation();
			break;
		case KEY_L:
		case KEY_l:
			favouriteLocationsMenu();
			break;
		case KEY_S:
		case KEY_s:
			saveLocation();
			break;
		case KEY_P:
		case KEY_p:
			pauseMenu();
			break;
		case KEY_Q:
		case KEY_q:
			update_mtx.lock();
			system("cls");
			std::cout << "\n         Exiting programm..." << std::endl;
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