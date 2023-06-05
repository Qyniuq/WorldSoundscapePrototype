#include "WorldSoundscape.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include <random>

WorldSoundscape::WorldSoundscape() : weather{false, "none", "none", "none"} {

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
}

WorldSoundscape::~WorldSoundscape()
{
	alDeleteEffects(1, &reverbEffect);
	alDeleteAuxiliaryEffectSlots(1, &reverbEffectSlot);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void WorldSoundscape::setUserEnterLocation()
{
	std::cout << "\nPlease enter the city name: ";
	std::getline(std::cin, weather.city_input);
	weather.city = weather.city_input;
	weather.whiteSpaceURLManager(weather.city);
	std::cout << "Please enter the country code: ";
	std::cin >> weather.country_code;

	if (weather.country_code == "US") {
		std::cout << "Please enter the state code for more accuracy: ";
		std::cin >> weather.state_code;
	}
	else {
		weather.state_code = "none";
	}
	weather.user_location = false;
}

void WorldSoundscape::setUserlocation()
{
	weather.user_location = true;
}

void WorldSoundscape::setRandomLocation() {
	if(weather.cities.IsNull()) {
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
	//shared.exclusive_lock()
	weather.city = city_name;
	weather.city_input = city_name;
	weather.whiteSpaceURLManager(weather.city);
	weather.country_code = country_code;
	weather.user_location = false;
	random_location_update = true;
	cv3.notify_one();
	//shared.exclusive_unlock()
}

void WorldSoundscape::mainMenu(){
	std::cout << "Welcome to World Soundscape, press the following keys to select an option:\n" <<
		"R: Random Location\n" <<
		"L: Enter Location\n" <<
		"G: User Location\n" <<
		"X: Exit\n";
		
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

		case KEY_G:
		case KEY_g:
			system("cls");
			std::cout << "U S E R   L O C A T I O N" << std::endl;
			setUserlocation();
			valid_response = true;
			break;

		case KEY_L:
		case KEY_l:
			system("cls");
			std::cout << "E N T E R   L O C A T I O N" << std::endl;
			setUserEnterLocation();
			valid_response = true;
			break;

		case KEY_X:
		case KEY_x:
			system("cls");
			std::cout << "Exiting programm..." << std::endl;
			stop_flag = true;
			cv.notify_one();
			exit_World_Soundscape = true;
			valid_response = true;
			break;

		default:
			std::cout << "Wrong command, try again..." << std::endl;
		}
	}
}

void WorldSoundscape::displayWeather(Weather& weather, std::vector<std::string>&notes_played){
		std::mutex mtx;
	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		shmtx.lock(); //shared.lock()
		system("CLS");
		weather.display();
		std::cout << "\n";
		for (auto& n : notes_played)
			std::cout << n << " ";
		shmtx.unlock();//shared.unlock()
		cv.wait_for(lock, std::chrono::milliseconds(1000));
	}
}

void WorldSoundscape::updateWeather(Weather& weather){
	std::mutex mtx;
	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		float reverbGain = static_cast<float>(weather.humidity) / 100.0f;
		alEffectf(reverbEffect, AL_EAXREVERB_GAIN, reverbGain);
		alAuxiliaryEffectSloti(reverbEffectSlot, AL_EFFECTSLOT_EFFECT, reverbEffect);		
		cv3.wait_for(lock, std::chrono::milliseconds(5000));
		if(!stop_flag) {//shared.lock()
			weather.callAllAPIs();
			//shared.unlock()
			if (random_location_update) {
				cv2.notify_one();
				random_location_update = false;
			}
		}		
	}
}

void WorldSoundscape::play_notes(Instrument& instrument, Weather& weather, std::vector<std::string>& notes_played){
	srand(time(0));
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::vector <notes> mode = setScaleForInstrument(instrument, weather);
	int r = rand() % mode.size();
	std::mutex mtx;

	while (!stop_flag) {
		std::unique_lock<std::mutex> lock(mtx);
		if (notes_played.size() >= 20)
			notes_played.clear();
		while (random_location_update) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
		//shared.lock()
		instrument.sounds[mode[r]].play();
		std::shared_lock sh_lock(shmtx);
		std::cout << instrument.sounds[mode[r]].sharp_name << " ";
		notes_played.push_back(instrument.sounds[mode[r]].sharp_name);
		sh_lock.unlock();
		//shared.unlock()

		int sleep_time;
		if (weather.wind_speed != 0) {
			sleep_time = static_cast<int>((rand() % instrument.sleep_value) / (weather.wind_speed / 1.7));
		}
		else {
			sleep_time = static_cast<int>((rand() % instrument.sleep_value) + 1000);
		}		
		cv2.wait_for(lock, std::chrono::milliseconds(sleep_time));
		mode = setScaleForInstrument(instrument, weather);
		if(!random_location_update)wait = false;
		r = rand() % mode.size();
	}
}

void WorldSoundscape::initMusic(){
	Instrument Jaguar = CreateJaguarGuitar(reverbEffectSlot);
	Instrument FemaleVoice = CreateFemaleVoice(reverbEffectSlot);

	std::vector<std::string > notes_played;
	std::jthread update_weather_thread(&WorldSoundscape::updateWeather, this, std::ref(weather));
	std::jthread display_weather_thread(&WorldSoundscape::displayWeather, this, std::ref(weather), std::ref(notes_played));
	std::jthread jaguar_guitar_thread(&WorldSoundscape::play_notes, this, std::ref(Jaguar), std::ref(weather), std::ref(notes_played));
	std::jthread female_voice_thread(&WorldSoundscape::play_notes, this, std::ref(FemaleVoice), std::ref(weather), std::ref(notes_played));
	std::jthread keyboard_listener_thread(&WorldSoundscape::keyboard_listener, this);
}

void WorldSoundscape::keyboard_listener() {

	int value{ 0 };
	while (value != KEY_X) {
		switch (_getch()) {
		case KEY_R:
		case KEY_r:
			wait = true;
			cv.notify_all();
			system("cls");
			std::cout << "New Random Location..." << std::endl;
			setRandomLocation();
			break;

		case KEY_L:
		case KEY_l:
			system("cls");
			std::cout << "New User Location..." << std::endl;
			cv.notify_all();
			stop_flag = true;
			break;

		case KEY_M:
		case KEY_m:
			system("cls");
			std::cout << "Back to menu..." << std::endl;
			cv.notify_all();
			stop_flag = true;
			break;

		case KEY_X:
		case KEY_x:
			system("cls");
			std::cout << "Exiting programm...\nPlease wait..." << std::endl;
			stop_flag = true;
			cv.notify_all();
			exit_World_Soundscape = true;
			value = KEY_X;
			break;

		default:
			std::cout << "Wrong command" << std::endl;
		}
	}
}
