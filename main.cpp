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
#include "WorldSoundscape.h"

std::atomic<bool> stop_flag{ false };


int main() {

	WorldSoundscape MyApp;
	MyApp.mainMenu();
	while(!MyApp.exit_World_Soundscape){
	MyApp.weather.callAllAPIs();
	MyApp.updateScale();
	if(!MyApp.exit_World_Soundscape) 
		MyApp.initMusic();
	}
	return 0;
}