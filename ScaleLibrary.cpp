#include "ScaleLibrary.h"
#include "utilities.h"
#include "NoteNames.h"

//Major modes
std::vector <intervals> ionian{ T, T, sT, T, T, T, sT };
std::vector <intervals> dorian{ T, sT, T, T, T, sT, T };
std::vector <intervals> phrygian{ sT, T, T, T, sT, T, T };
std::vector <intervals> lydian{ T, T, T, sT, T, T, sT };
std::vector <intervals> mixolydian{ T, T, sT, T, T, sT, T };
std::vector <intervals> aeolian{ T, sT, T, T, sT, T, T };
std::vector <intervals> locrian{ sT, T, T, sT, T, T, T };

//Melodic Minor modes
std::vector <intervals> melodic_minor{ T, sT, T, T, T, T, sT };
std::vector <intervals> dorian_b2{ sT, T, T, T, T, sT, T };
std::vector <intervals> lydian_augmented{ T, T, T, T, sT, T, sT };
std::vector <intervals> lydian_dominant{ T, T, T, sT, T, sT, T };
std::vector <intervals> mixolydian_b6{ T, T, sT, T, sT, T, T };
std::vector <intervals> aeolian_b5{ T, sT, T, sT, T, T, T };
std::vector <intervals> superlocrian{ sT, T, sT, T, T, T, T };

//Harmonic Minor modes
std::vector <intervals> harmonic_minor{ T, sT, T, T, sT, TsT, sT };
std::vector <intervals> locrian_6{ sT, T, T, sT, TsT, sT, T };
std::vector <intervals> ionian_sharp5{ T, T, sT, TsT, sT, T, sT };
std::vector <intervals> dorian_sharp4{ T, sT, TsT, sT, T, sT, T };
std::vector <intervals> phrygian_major{ sT, TsT, sT, T, sT, T, T };
std::vector <intervals> lydian_sharp2{ TsT, sT, T, sT, T, T, sT };
std::vector <intervals> ultralocrian{ sT, T, sT, T, T, sT, TsT };

//Harmonic Major modes
std::vector <intervals> harmonic_major{ T, T, sT, T, sT, TsT, sT };
std::vector <intervals> dorian_b5{ T, sT, T, sT, TsT, sT, T };
std::vector <intervals> superphrygian{ sT, T, sT, TsT, sT, T, T };
std::vector <intervals> lydian_b3{ T, sT, TsT, sT, T, T, sT };
std::vector <intervals> mixolydian_b2{ sT, TsT, sT, T, T, sT, T };
std::vector <intervals> lydian_augmented_sharp2{ TsT, sT, T, T, sT, T, sT };
std::vector <intervals> locrian_bb7{ sT, T, T, sT, T, sT, TsT };

//Double Harmonic modes
std::vector <intervals> double_harmonic_major{ sT, TsT, sT, T, sT, TsT, sT };
std::vector <intervals> lydian_sharp2_sharp6{ TsT, sT, T, sT, TsT, sT, sT };
std::vector <intervals> phrygian_b4_bb7{ sT, T, sT, TsT, sT, sT, TsT };
std::vector <intervals> double_harmonic_minor{ T, sT, TsT, sT, sT, TsT, sT };
std::vector <intervals> mixolydian_b5_b9{ sT, TsT, sT, sT, TsT, sT, T };
std::vector <intervals> ionian_augmented_sharp2{ TsT, sT, sT, TsT, sT, T, sT };
std::vector <intervals> locrian_bb3_bb7{ sT, sT, TsT, sT, T, sT, TsT };

//Hungarian Major modes
std::vector <intervals> lydian_dominant_sharp2{ TsT, sT, T, sT, T, sT, T };
std::vector <intervals> ultralocrian_bb6{ sT, T, sT, T, sT, T, TsT };
std::vector <intervals> harmonic_minor_b5{ T, sT, T, sT, T, TsT, sT };
std::vector <intervals> superlocrian_6{ sT, T, sT, T, TsT, sT, T };
std::vector <intervals> melodic_minor_sharp5{ T, sT, T, TsT, sT, T, sT };
std::vector <intervals> phrygian_sharp4_6{ sT, T, TsT, sT, T, sT, T };
std::vector <intervals> lydian_augmented_sharp3{ T, TsT, sT, T, sT, T, sT };

//Romanian Major modes
std::vector <intervals> romanian_major{ sT, TsT, T, sT, T, sT, T };
std::vector <intervals> super_lydian_augmented_6{ TsT, T, sT, T, sT, T, sT };
std::vector <intervals> locrian_9_bb7{ T, sT, T, sT, T, sT, TsT };
std::vector <intervals> superlocrian_bb6{ sT, T, sT, T, sT, TsT, T };
std::vector <intervals> melodic_minor_b5{ T, sT, T, sT, TsT, T, sT };
std::vector <intervals> superphrygian_6{ sT, T, sT, TsT, T, sT, T };
std::vector <intervals> lydian_augmented_b3{ T, sT, TsT, T, sT, T, sT };

//Symetric Scales
std::vector <intervals> whole_tone{ T, T, T, T, T, T };
std::vector <intervals> diminished_T_sT{ T, sT, T, sT, T, sT, T, sT };
std::vector <intervals> diminished_sT_T{ sT, T, sT, T, sT, T, sT, T };
std::vector <intervals> messiaen_third_mode{ T, sT, sT, T, sT, sT, T, sT, sT };
std::vector <intervals> messiaen_fourth_mode{ sT, sT, TsT, sT, sT, sT, TsT, sT };
std::vector <intervals> messiaen_fifth_mode{ sT, TT, sT, sT, TT, sT };
std::vector <intervals> messiaen_sixth_mode{ T, T, sT, sT, T, T, sT, sT };
std::vector <intervals> messiaen_seventh_mode{ sT, sT, sT, T, sT, sT, sT, sT, T, sT };
std::vector <intervals> augmented{ TsT, sT, TsT, sT, TsT, sT };
std::vector <intervals> atonal{ sT, sT, sT, sT, sT, sT, sT, sT, sT, sT, sT, sT };

std::vector <intervals> phrygian_major7{ sT, T, T, T, sT, TsT, sT };
std::vector <intervals> mixolydian_b2_b5{ sT, TsT, sT, sT, TsT, sT, T };

std::vector<std::vector<intervals>> getAll7thNoteScales() {
	std::vector<std::vector<intervals>> all_scales;

	// Major modes
	all_scales.push_back(ionian);
	all_scales.push_back(dorian);
	all_scales.push_back(phrygian);
	all_scales.push_back(lydian);
	all_scales.push_back(mixolydian);
	all_scales.push_back(aeolian);
	all_scales.push_back(locrian);

	// Melodic Minor modes
	all_scales.push_back(melodic_minor);
	all_scales.push_back(dorian_b2);
	all_scales.push_back(lydian_augmented);
	all_scales.push_back(lydian_dominant);
	all_scales.push_back(mixolydian_b6);
	all_scales.push_back(aeolian_b5);
	all_scales.push_back(superlocrian);

	// Harmonic Minor modes
	all_scales.push_back(harmonic_minor);
	all_scales.push_back(locrian_6);
	all_scales.push_back(ionian_sharp5);
	all_scales.push_back(dorian_sharp4);
	all_scales.push_back(phrygian_major);
	all_scales.push_back(lydian_sharp2);
	all_scales.push_back(ultralocrian);

	// Harmonic Major modes
	all_scales.push_back(harmonic_major);
	all_scales.push_back(dorian_b5);
	all_scales.push_back(superphrygian);
	all_scales.push_back(lydian_b3);
	all_scales.push_back(mixolydian_b2);
	all_scales.push_back(lydian_augmented_sharp2);
	all_scales.push_back(locrian_bb7);

	// Double Harmonic modes
	all_scales.push_back(double_harmonic_major);
	all_scales.push_back(lydian_sharp2_sharp6);
	all_scales.push_back(phrygian_b4_bb7);
	all_scales.push_back(double_harmonic_minor);
	all_scales.push_back(mixolydian_b5_b9);
	all_scales.push_back(ionian_augmented_sharp2);
	all_scales.push_back(locrian_bb3_bb7);

	// Hungarian Major modes
	all_scales.push_back(lydian_dominant_sharp2);
	all_scales.push_back(ultralocrian_bb6);
	all_scales.push_back(harmonic_minor_b5);
	all_scales.push_back(superlocrian_6);
	all_scales.push_back(melodic_minor_sharp5);
	all_scales.push_back(phrygian_sharp4_6);
	all_scales.push_back(lydian_augmented_sharp3);

	// Romanian Major modes
	all_scales.push_back(romanian_major);
	all_scales.push_back(super_lydian_augmented_6);
	all_scales.push_back(locrian_9_bb7);
	all_scales.push_back(superlocrian_bb6);
	all_scales.push_back(melodic_minor_b5);
	all_scales.push_back(superphrygian_6);
	all_scales.push_back(lydian_augmented_b3);

	all_scales.push_back(phrygian_major7);
	all_scales.push_back(mixolydian_b2_b5);

	return all_scales;
}

std::vector<std::string> getAll7thNoteScaleNames() {
	std::vector<std::string> scale_names;

	// Major modes
	scale_names.push_back("Ionian");
	scale_names.push_back("Dorian");
	scale_names.push_back("Phrygian");
	scale_names.push_back("Lydian");
	scale_names.push_back("Mixolydian");
	scale_names.push_back("Aeolian");
	scale_names.push_back("Locrian");

	// Melodic Minor modes
	scale_names.push_back("Melodic Minor");
	scale_names.push_back("Dorian b2");
	scale_names.push_back("Lydian #5");
	scale_names.push_back("Lydian Dominant");
	scale_names.push_back("Mixolydian b6");
	scale_names.push_back("Aeolian b5");
	scale_names.push_back("Super Locrian");

	// Harmonic Minor modes
	scale_names.push_back("Harmonic Minor");
	scale_names.push_back("Locrian 6");
	scale_names.push_back("Ionian #5");
	scale_names.push_back("Dorian #4");
	scale_names.push_back("Phrygian Major");
	scale_names.push_back("Lydian #2");
	scale_names.push_back("Ultra Locrian");

	// Harmonic Major modes
	scale_names.push_back("Harmonic Major");
	scale_names.push_back("Dorian b5");
	scale_names.push_back("Super Phrygian");
	scale_names.push_back("Lydian b3");
	scale_names.push_back("Mixolydian b2");
	scale_names.push_back("Lydian Augmented #2");
	scale_names.push_back("Locrian bb7");

	// Double Harmonic modes
	scale_names.push_back("Double Harmonic Major");
	scale_names.push_back("Lydian #2 #6");
	scale_names.push_back("Phrygian b4 bb7");
	scale_names.push_back("Double Harmonic Minor");
	scale_names.push_back("Mixolydian b5 b9");
	scale_names.push_back("Ionian Augmented #2");
	scale_names.push_back("Locrian bb3 bb7");

	// Hungarian Major modes
	scale_names.push_back("Lydian Dominant #2");
	scale_names.push_back("Ultra Locrian bb6");
	scale_names.push_back("Harmonic Minor b5");
	scale_names.push_back("Super Locrian 6");
	scale_names.push_back("Melodic Minor #5");
	scale_names.push_back("Phrygian #4 6");
	scale_names.push_back("Lydian Augmented #3");

	// Romanian Major modes
	scale_names.push_back("Romanian Major");
	scale_names.push_back("Super Lydian Augmented 6");
	scale_names.push_back("Locrian 9 bb7");
	scale_names.push_back("Super Locrian bb6");
	scale_names.push_back("Melodic Minor b5");
	scale_names.push_back("Super Phrygian 6");
	scale_names.push_back("Lydian Augmented b3");

	scale_names.push_back("Phrygian 7");
	scale_names.push_back("Mixolydian b2 b5");

	return scale_names;
}

std::vector<std::vector<intervals>> getAllRareScales() {
	std::vector<std::vector<intervals>> rare_scales {
		{ T, T, T, T, T, T },                    // whole_tone
		{ T, sT, T, sT, T, sT, T, sT },          // diminished_T_sT
		{ sT, T, sT, T, sT, T, sT, T },          // diminished_sT_T
		{ T, sT, sT, T, sT, sT, T, sT, sT },     // messiaen_third_mode
		{ sT, sT, TsT, sT, sT, sT, TsT, sT },    // messiaen_fourth_mode
		{ sT, TT, sT, sT, TT, sT },              // messiaen_fifth_mode
		{ T, T, sT, sT, T, T, sT, sT },          // messiaen_sixth_mode
		{ sT, sT, sT, T, sT, sT, sT, sT, T, sT }, // messiaen_seventh_mode
		{ TsT, sT, TsT, sT, TsT, sT },           // augmented
		{ sT, sT, sT, sT, sT, sT, sT, sT, sT, sT, sT, sT } // atonal
	};

	return rare_scales;
}

std::vector<std::string> getAllRareScaleNames() {
	std::vector<std::string> rareScaleNames {
		"Whole Tone",
			"Diminished T sT",
			"Diminished sT T",
			"Messiaen Third Mode",
			"Messiaen Fourth Mode",
			"Messiaen Fifth Mode",
			"Messiaen Sixth Mode",
			"Messiaen Seventh Mode",
			"Augmented",
			"Atonal"
	};
	return rareScaleNames;
}

int getWeatherPoints(const Weather& weather);

Scale getScaleForNight(const int& points) {
	Scale result;

	if (points >= 0) {
		result.matrix = melodic_minor;
		result.name = "Melodic Minor";
	}

	else if (points == -1) {
		result.matrix = harmonic_minor;
		result.name = "Harmonic Minor";
	}

	else if (points == -2) {
		result.matrix = double_harmonic_minor;
		result.name = "Double Harmonic Minor";
	}

	else if (points == -3) {
		result.matrix = melodic_minor_b5;
		result.name = "Melodic Minor b5";
	}

	else if (points == -4) {
		result.matrix = harmonic_minor_b5;
		result.name = "Harmonic Minor b5";
	}

	else if (points == -5) {
		result.matrix = locrian_9_bb7;
		result.name = "Locrian natural 9 bb7";
	}

	else if (points == -6) {
		result.matrix = diminished_T_sT;
		result.name = "Diminished T - ST";
	}

	else if (points == -7) {
		result.matrix = messiaen_third_mode;
		result.name = "Messiaen's Third Limited Transposition Mode";
	}

	else if (points <= -8) {
		result.matrix = atonal;
		result.name = "Atonal";
	}

	return result;
}

Scale getScaleForAstronomicalDawn(const int& points) {
	Scale result;

	if (points >= 0) {
		result.matrix = lydian_sharp2;
		result.name = "Lydian #2";
	}

	else if (points == -1) {
		result.matrix = lydian_dominant_sharp2;
		result.name = "Lydian Dominant #2";
	}

	else if (points == -2) {
		result.matrix = lydian_sharp2_sharp6;
		result.name = "Lydian #2 #6";
	}

	else if (points == -3) {
		result.matrix = lydian_augmented_sharp2;
		result.name = "Lydian Augmented #2";
	}

	else if (points == -4) {
		result.matrix = lydian_b3;
		result.name = "Lydian b3";
	}

	else if (points == -5) {
		result.matrix = superlocrian_6;
		result.name = "Superlocrian natural 6";
	}

	else if (points == -6) {
		result.matrix = superlocrian;
		result.name = "Superlocrian";
	}

	else if (points == -7) {
		result.matrix = ultralocrian;
		result.name = "Ultralocrian";
	}

	else if (points <= -8) {
		result.matrix = ultralocrian_bb6;
		result.name = "Ultralocrian bb6";
	}

	return result;
}

Scale getScaleForNauticalDawn(const int& points) {
	Scale result;

	if (points >= 0) {
		result.matrix = harmonic_major;
		result.name = "Harmonic Major";
	}

	else if (points == -1) {
		result.matrix = ionian_sharp5;
		result.name = "Ionian #5";
	}

	else if (points == -2) {
		result.matrix = mixolydian_b6;
		result.name = "Mixolydian b6";
	}

	else if (points == -3) {
		result.matrix = augmented;
		result.name = "Augmented";
	}

	else if (points == -4) {
		result.matrix = double_harmonic_major;
		result.name = "Double Harmonic Major";
	}

	else if (points == -5) {
		result.matrix = mixolydian_b2;
		result.name = "Mixolydian b2";
	}

	else if (points == -6) {
		result.matrix = phrygian_major;
		result.name = "Phrygian Major";
	}

	else if (points == -7) {
		result.matrix = phrygian_major7;
		result.name = "Phrygian #7";
	}

	else if (points <= -8) {
		result.matrix = mixolydian_b2_b5;
		result.name = "Mixolydian b2 b5";
	}

	return result;
}

Scale getScaleForCivilDawn(const int& points) {
	Scale result;

	if (points >= -2) {
		result.matrix = lydian;
		result.name = "Lydian";
	}

	else if (points > -6) {
		result.matrix = lydian_dominant;
		result.name = "Lydian Dominant";
	}

	else if (points > -8) {
		result.matrix = lydian_augmented;
		result.name = "Lydian Augmented";
	}

	else if (points <= -8) {
		result.matrix = whole_tone;
		result.name = "Whole Tone";
	}

	return result;
}

Scale getScaleForMorning(const int& points) {
	Scale result;

	if (points >= -2) {
		result.matrix = ionian;
		result.name = "Ionian";
	}

	else if (points == -3) {
		result.matrix = mixolydian;
		result.name = "Mixolydian";
	}

	else if (points < -3) {
		result.matrix = dorian;
		result.name = "Dorian";
	}

	return result;
}

Scale getScaleForAfternoon(const int& points) {
	Scale result;

	if (points >= -2) {
		result.matrix = mixolydian;
		result.name = "Mixolydian";
	}

	else if (points == -3) {
		result.matrix = dorian;
		result.name = "Dorian";
	}

	else if (points < -3) {
		result.matrix = aeolian;
		result.name = "Aeolian";
	}

	return result;
}

Scale getScaleForCivilDusk(const int& points) {
	Scale result;

	if (points >= -2) {
		result.matrix = dorian;
		result.name = "Dorian";
	}

	else if (points > -7) {
		result.matrix = aeolian;
		result.name = "Aeolian";
	}

	else if (points <= -7) {
		result.matrix = phrygian;
		result.name = "Phrygian";
	}

	return result;
}

Scale getScaleForNauticalDusk(const int& points) {
	Scale result;

	if (points >= -1) {
		result.matrix = aeolian;
		result.name = "Aeolian";
	}

	else if (points > -3) {
		result.matrix = harmonic_minor;
		result.name = "Harmonic Minor";
	}

	else if (points > -5) {
		result.matrix = double_harmonic_minor;
		result.name = "Double Harmonic Minor";
	}

	else if (points == -5) {
		result.matrix = aeolian_b5;
		result.name = "Aeolian b5";
	}

	else if (points == -6) {
		result.matrix = melodic_minor_b5;
		result.name = "Melodic Minor b5";
	}

	else if (points == -7) {
		result.matrix = harmonic_minor_b5;
		result.name = "Harmonic Minor b5";
	}

	else if (points <= -8) {
		result.matrix = diminished_T_sT;
		result.name = "Phrygian Major";
	}

	return result;
}

Scale getScaleForAstronomicalDusk(const int& points) {
	Scale result;

	if (points >= -1) {
		result.matrix = phrygian;
		result.name = "Phrygian";
	}

	else if (points >= -3) {
		result.matrix = superphrygian;
		result.name = "Superphrygian";
	}

	else if (points >= -5) {
		result.matrix = phrygian_b4_bb7;
		result.name = "Superphrygian bb7";
	}

	else if (points >= -7) {
		result.matrix = phrygian_sharp4_6;
		result.name = "Phrygian #4 natural 6";
	}

	else if (points <= -8) {
		result.matrix = diminished_sT_T;
		result.name = "Diminished ST - T";
	}
	return result;
}

Scale getScaleMatrix(Weather& weather) {
	Scale result;

	int points = getWeatherPoints(weather);
	weather.points = points;
	int temperature = weather.temperature != 0 ? static_cast<int>(weather.temperature) : static_cast<int>(weather.feels_like);
	temperature -= 273;

	switch (weather.status) {

	case Weather::DaylightStatus::Night:
		if (temperature < 0) {
			points += (temperature / 10);
		}
		result = getScaleForNight(points);
		break;

	case Weather::DaylightStatus::Astronomical_Dawn:
		result = getScaleForAstronomicalDawn(points);
		break;

	case Weather::DaylightStatus::Nautical_Dawn:
		result = getScaleForNauticalDawn(points);
		break;

	case Weather::DaylightStatus::Civil_Dawn:
		result = getScaleForCivilDawn(points);
		break;

	case Weather::DaylightStatus::Morning:
		result = getScaleForMorning(points);
		break;

	case Weather::DaylightStatus::Afternoon:
		result = getScaleForAfternoon(points);
		break;

	case Weather::DaylightStatus::Civil_Dusk:
		result = getScaleForCivilDusk(points);
		break;

	case Weather::DaylightStatus::Nautical_Dusk:
		result = getScaleForNauticalDusk(points);
		break;

	case Weather::DaylightStatus::Astronomical_Dusk:
		result = getScaleForAstronomicalDusk(points);
		break;

	default:
		std::cerr << "Error in getScaleMatrix() - weather class has no daylight status" << std::endl;
		break;
	}
	return result;
}

int getWeatherPoints(const Weather& weather) {
	int points{ 0 };

	//C L O U D S
	if (weather.cloudiness > 24)
		points -= 1;
	if (weather.cloudiness > 50)
		points -= 1;
	if (weather.cloudiness > 84)
		points -= 1;

	//A T M O S P H E R E
	if (weather.weather_description == "mist" || weather.visibility < 2000 && weather.visibility >= 1000)
		points -= 2;
	else if (weather.weather_description == "smoke")
		points -= 2;
	else if (weather.weather_description == "haze")
		points -= 1;
	else if (weather.weather_description == "sand/dust whirls")
		points -= 2;
	else if (weather.weather_description == "fog" || weather.visibility < 1000)
		points -= 4;
	else if (weather.weather_description == "sand")
		points -= 5;
	else if (weather.weather_description == "dust")
		points -= 5;
	else if (weather.weather_description == "volcanic ash")
		points -= 4;
	else if (weather.weather_description == "squalls")
		points -= 4;
	else if (weather.weather_description == "tornado")
		points -= 8;

	//S N O W
	if (weather.weather_main == "snow") {
		if (weather.weather_description == "light snow")
			points += 2;
		else if (weather.weather_description == "snow")
			points += 3;
		else if (weather.weather_description == "heavy snow")
			points += 4;
		else if (weather.weather_description == "sleet")
			points += 1;
		else if (weather.weather_description == "light shower sleet")
			points += 2;
		else if (weather.weather_description == "shower sleet")
			points += 3;
		else if (weather.weather_description == "light rain and snow")
			points -= 1;
		else if (weather.weather_description == "rain and snow")
			points -= 2;
		else if (weather.weather_description == "light shower snow")
			points += 2;
		else if (weather.weather_description == "shower snow")
			points += 3;
		else if (weather.weather_description == "heavy shower snow")
			points += 5;
	}

	//R A I N
	else if (weather.weather_main == "rain") {
		if (weather.weather_description == "light rain")
			points -= 2;
		else if (weather.weather_description == "moderate rain")
			points -= 3;
		else if (weather.weather_description == "heavy intensity rain")
			points -= 4;
		else if (weather.weather_description == "very heavy rain")
			points -= 5;
		else if (weather.weather_description == "extreme rain")
			points -= 6;
		else if (weather.weather_description == "freezing rain")
			points -= 7;
		else if (weather.weather_description == "light intensity shower rain")
			points -= 2;
		else if (weather.weather_description == "shower rain")
			points -= 3;
		else if (weather.weather_description == "heavy intensity shower rain")
			points -= 5;
		else if (weather.weather_description == "ragged shower rain")
			points -= 6;
	}

	//D R I Z Z L E
	else if (weather.weather_main == "drizzle") {
		if (weather.weather_description == "light intensity drizzle")
			points -= 1;
		else if (weather.weather_description == "drizzle")
			points -= 2;
		else if (weather.weather_description == "heavy intensity drizzle")
			points -= 3;
		else if (weather.weather_description == "light intensity drizzle rain")
			points -= 1;
		else if (weather.weather_description == "drizzle rain")
			points -= 2;
		else if (weather.weather_description == "heavy intensity drizzle rain")
			points -= 3;
		else if (weather.weather_description == "shower rain and drizzle")
			points -= 2;
		else if (weather.weather_description == "heavy shower rain and drizzle")
			points -= 3;
		else if (weather.weather_description == "shower drizzle")
			points -= 3;
	}

	//T H U N D E R S T O R M
	else if (weather.weather_main == "thunderstorm") {
		if (weather.weather_description == "thunderstorm with light rain")
			points -= 3;
		else if (weather.weather_description == "thunderstorm with rain")
			points -= 4;
		else if (weather.weather_description == "thunderstorm with heavy rain")
			points -= 5;
		else if (weather.weather_description == "light thunderstorm")
			points -= 2;
		else if (weather.weather_description == "thunderstorm")
			points -= 4;
		else if (weather.weather_description == "heavy thunderstorm")
			points -= 6;
		else if (weather.weather_description == "ragged thunderstorm")
			points -= 8;
		else if (weather.weather_description == "thunderstorm with light drizzle")
			points -= 2;
		else if (weather.weather_description == "thunderstorm with drizzle")
			points -= 4;
		else if (weather.weather_description == "thunderstorm with heavy drizzle")
			points -= 5;
	}
	return points;
}

std::string noteNumberToString(int Note) {
	std::string result;

	Note = getNumberInRange(0, 11, Note);

	switch (Note) {
	case 0:
		result= "C";
		break;
	case 1:
		result= "C#";
		break;
	case 2:
		result= "D";
		break;
	case 3:
		result= "D#";
		break;
	case 4:
		result= "E";
		break;
	case 5:
		result= "F";
		break;
	case 6:
		result= "F#";
		break;
	case 7:
		result= "G";
		break;
	case 8:
		result= "G#";
		break;
	case 9:
		result= "A";
		break;
	case 10:
		result= "A#";
		break;
	case 11:
		result= "B";
		break;
	default:
		result= "Unknown";
		break;
	}

	return result;
}

int getRoot(Weather &weather) {

	int tonic = weather.timezone / 3600;
	if (weather.wind_speed == 0)
		return tonic;
	else{
		int temperature;
		if (weather.temperature != 0)
			temperature = (static_cast<int>(weather.temperature - 273.15) -14);
		else {
			temperature = (static_cast<int>(weather.feels_like - 273.15) -14);
		}

		int wind_level = getNumberInRange(0, 6, static_cast<int>(weather.wind_speed * 3.6)/4);

		scale_degree interval {octave};
		switch (wind_level) {
		case 0:
			interval = fifth;
			break;
		case 1:
			interval = second;
			break;
		case 2:
			interval = sixth;
			break;
		case 3:
			interval = third;
			break;
		case 4:
			interval = seventh;
			break;
		case 5:
			interval = diminished_fifth;
			break;
		case 6:
			interval = minor_second;
			break;
		default:
			std::cout << "some error ocurred in wind level switch";
		}

		tonic += (temperature * interval) % 12;
		return tonic;
	}
}

std::vector<std::string>getAlterationsNames(std::vector<notes> vnotes) {
	std::vector<std::string> sharp {"C#", "D#", "E#", "F#", "G#", "A#", "B#"};
	std::vector<std::string> natural {"C", "D", "E", "F", "G", "A", "B"};
	std::vector<std::string> flat {"Cb", "Db", "Eb", "Fb", "Gb", "Ab", "Bb"};
	
	return sharp;
}

Scale getGeneralScale(Weather& weather, bool flat_preference, bool enharmonics, bool double_alterations, bool enharmonics_tonics) {
	Scale result = getScaleMatrix(weather);
	weather.scale = result.name;
	result.root = getNumberInRange(0, 11, getRoot(weather));
	std::vector<int> scale {result.root};

	for (int i{ 0 }; i < result.matrix.size() - 1; ++i) {
		scale.push_back(scale.at(i) + result.matrix.at(i));
	}
	std::vector<std::string> names = getScaleNoteNames(scale, flat_preference, enharmonics, double_alterations, enharmonics_tonics);
	int i{ 0 };
	for (auto &n : scale) {
		n = getNumberInRange(0, 11, n);
		result.note_names[n] = names.at(i);
		++i;
	}
	weather.tonality = names.at(0);
	weather.scale_note_names = names;
	return result;
}

std::vector <notes> setScaleForInstrument(const Instrument& instrument, Scale s) {

	std::vector <notes> scale;
	int root = s.root;
	int note = getNumberInRange(instrument.min, instrument.min + 11, root);

	for (int i{ 0 }; note <= instrument.max; ++i) {
		if (i == s.matrix.size())
			i = 0;
		scale.push_back(static_cast<notes>(note));
		note += s.matrix[i];
	}

	if (instrument.min == -1) {
		note = (getNumberInRange(instrument.min, instrument.min + 11, root)) - s.matrix[s.matrix.size()-1]; 
		for (size_t i{ s.matrix.size() - 2 }; note >= instrument.min; --i) {
			if (i == -1)
				i = s.matrix.size() - 1;
			scale.push_back(static_cast<notes>(note));
			note -= s.matrix[i];
		}
	}

	return scale;
}
