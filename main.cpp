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
	while (!MyApp.exit_World_Soundscape) {
	while (!MyApp.startMusic && !MyApp.exit_World_Soundscape) {
		MyApp.mainMenu();
	}	
	if(!MyApp.exit_World_Soundscape && MyApp.startMusic)
		MyApp.initMusic();
	}
	return 0;
}