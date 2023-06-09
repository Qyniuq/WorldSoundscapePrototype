#include "Weather.h"
#include <iomanip>
#include "ScaleLibrary.h"
#include "CountryMap.h"

using namespace std::literals;

Weather::Weather(bool user_location, std::string city_input, std::string country_code_input, std::string state_code_input) : user_location{ user_location }, city{ city_input }, state_code{ state_code_input },
country_code{ country_code_input }, scale{ "none" }, tonality{ "none" }, points{ 0 }, city_input{ city_input }, weather_main{ "none" }, weather_description{ "none" }, temperature{ 0.0 }, feels_like{ 0.0 },
wind_speed{ 0.0 }, rain_vol{ 0.0 }, snow_vol{ 0.0 }, cloudiness{ 0 }, humidity{ 0 }, visibility{ 0 }, timezone{ 0 }, city_time{ "none" }, open_weather_call_count{ 0 }, sunrise_sunset_call_count{ 0 },
USNO_call_count{ 0 }, geoPLUGIN_call_count{ 0 }, data_wrong{ false }, day_offset{ 0 }, sunrise{ 0, *this }, sunset{ 0, *this }, current_time{ 0, *this }, civil_dawn{ 0, *this }, civil_dusk{ 0, *this },
nautical_dawn{ 0, *this }, nautical_dusk{ 0, *this }, astronomical_dawn{ 0, *this }, astronomical_dusk{ 0, *this }, solar_noon{ 0, *this }
{
    in_file.open("apicode.txt");
    if (!in_file) {
        std::cerr << "apicode.txt not found" << std::endl;
    }
    else {
        std::getline(in_file, api_key);
    }
    in_file.close();
    whiteSpaceURLManager(city);
    countryMap = createCountryMap();
}

size_t Weather::WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        std::cerr << "Out of memory" << std::endl;
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void Weather::callAPI(std::string URL, APIs API) {

    curl = curl_easy_init();
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

    res = curl_easy_perform(curl); //calls the API
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    else {
        doc.Parse(chunk.memory);

        if (!doc.IsNull()) {
            switch (API) {
            case APIs::geoPLUGIN:
                updateGeoLocation();
                break;

            case APIs::OpenWeather:
                updateOpenWeatherData();
                break;

            case APIs::SunriseAndSunset:
                if (data_is_wrong(day_offset)) {
                    data_wrong = true;
                }
                else {
                    data_wrong = false;
                    updateSunsetAndSunriseData();
                } break;
            case APIs::USNO:
                updateMoonPhase();
                break;
            }

        }
        else {
            std::cerr << "Error: JSON response is null" << std::endl;
        }
        doc.RemoveAllMembers();
        free(chunk.memory);
        curl_easy_cleanup(curl);
    }
}

void Weather::callAllAPIs() {
    static int count{ 0 };

    if (user_location) {
        URL = "http://www.geoplugin.net/json.gp?";
        callAPI(URL, APIs::geoPLUGIN);
        geoPLUGIN_call_count += 1;
    }

    if (state_code != "none") {
        URL = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + state_code + "," + country_code + "&appid=" + api_key;
    }
    else {
        URL = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + country_code + "&appid=" + api_key;
    }
    callAPI(URL, APIs::OpenWeather);
    open_weather_call_count += 1;

    time_t day_length = 86400;
    time_t local_time = time(0) + timezone;

    static int previous_timezone{ 0 };
    static double previous_lat{ 0. };
    static double previous_lon{ 0. };

    if (count == 0 || time(0) > end_of_UTC_day || timezone != previous_timezone || lat != previous_lat || lon != previous_lon) {
        do {
            int year = getYear(time(0));
            int month = getMonth(time(0));
            int day = getDay(local_time);

            if (day_offset == 1)
                day = getDay((local_time + day_length));
            else if (day_offset == -1)
                day = getDay((local_time - day_length));

            URL = "https://api.sunrise-sunset.org/json?lat=" + std::to_string(lat) + "&lng=" + std::to_string(lon) + "&formatted=0&date=" + std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
            callAPI(URL, APIs::SunriseAndSunset);
            country_name = getCountryName(countryMap, country_code);
            previous_lon = lon;
            previous_lat = lat;
            previous_timezone = timezone;
            sunrise_sunset_call_count += 1;
        } while (data_wrong); //Sunrise Sunset API does retrieve wrong data with one day offset for some countries on the edges of the timezone (+12/-12UTC);
    }

    if (count == 0 || time(0) > end_of_UTC_day || moon_phase.length() == 0) {
        int year = getYear(time(0));
        int month = getMonth(time(0));
        int day = getDay(time(0));

        URL = "https://aa.usno.navy.mil/api/rstt/oneday?date=" + std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day) + "%20" + "&coords=" + std::to_string(lat) + ",%20" + std::to_string(lon) + "&tz=0%20&dst=false";
        callAPI(URL, APIs::USNO);

        USNO_call_count += 1;
    }
    count += 1;
}

time_t Weather::getEndOfUTCDay() {

    time_t day_length = 86400;
    time_t local_time = time(0);
    time_t elapsed_time = local_time % day_length;
    time_t time_left = day_length - elapsed_time;

    return (local_time + time_left);
}

time_t Weather::getEndOfCurrentDay() {

    time_t day_length = 86400;
    time_t local_time = time(0) + timezone;
    time_t elapsed_time = local_time % day_length;
    time_t time_left = day_length - elapsed_time;

    return (local_time + time_left);
}

void Weather::updateGeoLocation() {

    std::string c = getStringData("geoplugin_city");
    if (c.length() == 0) c = getStringData("geoplugin_region");
    std::string c_code = getStringData("geoplugin_countryCode");
    setCity(c, c_code);
    city_input = c;
}

void Weather::updateSunsetAndSunriseData() {

    end_of_current_day = getEndOfCurrentDay();
    day_length = getIntSubData("results", "day_length");
    day_length_seconds = convertStringToTime(day_length);

    Time sunrise_temp(getStringSubData("results", "sunrise"), *this);
    sunrise = sunrise_temp;

    Time sunset_temp(getStringSubData("results", "sunset"), *this);
    sunset = sunset_temp;

    Time solar_noon_temp(getStringSubData("results", "solar_noon"), *this);
    solar_noon = solar_noon_temp;

    Time civil_dawn_temp(getStringSubData("results", "civil_twilight_begin"), *this);
    civil_dawn = civil_dawn_temp;

    Time nautical_dawn_temp(getStringSubData("results", "nautical_twilight_begin"), *this);
    nautical_dawn = nautical_dawn_temp;

    Time astronomical_dawn_temp(getStringSubData("results", "astronomical_twilight_begin"), *this);
    astronomical_dawn = astronomical_dawn_temp;

    Time civil_dusk_temp(getStringSubData("results", "civil_twilight_end"), *this);
    civil_dusk = civil_dusk_temp;

    Time nautical_dusk_temp(getStringSubData("results", "nautical_twilight_end"), *this);
    nautical_dusk = nautical_dusk_temp;

    Time astronomical_dusk_temp(getStringSubData("results", "astronomical_twilight_end"), *this);
    astronomical_dusk = astronomical_dusk_temp;

    day_light_status = getDayLightStatus(time(0));
}

void Weather::updateMoonPhase() {

    if (doc.HasMember("properties") && doc["properties"].IsObject()) {
        rapidjson::Value& properties_obj = doc["properties"];
        rapidjson::Value& data_obj = properties_obj["data"];

        if (data_obj.HasMember("curphase")) {
            moon_phase = data_obj["curphase"].GetString();
            moon_fracillum = data_obj["fracillum"].GetString();
            end_of_UTC_day = getEndOfUTCDay();
        }
        else {
            rapidjson::Value& closestphase_obj = data_obj["closestphase"];
            moon_phase = closestphase_obj["phase"].GetString();
            moon_fracillum = "none";
            end_of_UTC_day = getEndOfUTCDay();
        }
    }
    else
        moon_phase = "no current moonphase";
}

void Weather::updateOpenWeatherData() {

    setWeather();
    lon = getDoubleSubData("coord", "lon");
    lat = getDoubleSubData("coord", "lat");
    timezone = getIntData("timezone");
    time_offset = convertUTCtimeToString(timezone);
    visibility = getIntData("visibility");
    wind_speed = getDoubleSubData("wind", "speed");
    rain_vol = getDoubleSubData("rain", "1h");
    snow_vol = getDoubleSubData("snow", "1h");
    temperature = getDoubleSubData("main", "temp");
    feels_like = getDoubleSubData("main", "feels_like");
    pressure = getIntSubData("main", "pressure");
    humidity = getIntSubData("main", "humidity");
    cloudiness = getIntSubData("clouds", "all");
    humidity = getIntSubData("main", "humidity");
    city_time = convertTimeToLocale(time(0));
}

void Weather::display() {

    day_light_status = getDayLightStatus(time(0));
    city_time = convertTimeToLocale(time(0));
    int length{ 70 };
    std::cout << "\n";
    int space{ (length - (static_cast<int>(city_input.size()) + 2 + static_cast<int>(country_name.size()))) / 2 };

    std::cout << std::setprecision(9);
    std::cout << std::setw(space) << "" << city_input << " / " << country_name << "\n";
    std::cout << std::setfill('-') << std::setw(length) << "" << std::setfill(' ') << "\n";

    std::cout << "Temperature: ";
    if (temperature != 0) std::cout << temperature - 273.15 << "\370C / ";
    if (feels_like != 0) std::cout << "feels like " << feels_like - 273.15 << "\370C";
    std::cout << "\nWeather: " << weather_main;
    std::cout << " / " << weather_description << "\n";
    std::cout << "Wind speed: " << wind_speed * 3.6 << " km/h\n";
    std::cout << "Pressure: " << pressure << "hPa\n";
    std::cout << "Visibility: " << visibility << " meters\n";
    std::cout << "Timezone offset: UTC" << time_offset << " hours\n";
    std::cout << "Raining volume: " << rain_vol << " mm/hr\n";;
    std::cout << "Snow volume: " << snow_vol << " mm/h\n";
    std::cout << "Humidity: " << humidity << "%\n";
    std::cout << "Cloudiness: " << cloudiness << "%\n";

    std::cout << "Astronomical Dawn begins at: " << astronomical_dawn.local_str << "\n";
    std::cout << "Nautical Dawn begins at: " << nautical_dawn.local_str << "\n";
    std::cout << "Civil Dawn begins at: " << civil_dawn.local_str << "\n";
    std::cout << "Sunrise at: " << sunrise.local_str << "\n";
    std::cout << "Solar noon at: " << solar_noon.local_str << "\n";
    std::cout << "Sunset at: " << sunset.local_str << "\n";
    std::cout << "Civil dusk ends at: " << civil_dusk.local_str << "\n";
    std::cout << "Nautical dusk ends at: " << nautical_dusk.local_str << "\n";
    std::cout << "Astronomical dusk ends at: " << astronomical_dusk.local_str << "\n";

    std::cout << "Moon phase: " << moon_phase; if (moon_fracillum != "none") std::cout << " " << moon_fracillum << "\n";
    std::cout << "Latitude: " << lat << " / " << "Longitude: " << lon << "\n";
    std::cout << "Points: " << points << "\nScale: " << tonality << " " << scale << "\n";
    std::cout << std::right << std::setw(length) << day_light_status << std::left << "\n";
    std::cout << std::setw(length - (14 + city_time.length())) << "" << "Current time: " << city_time << "\n";
    std::cout << std::setfill('-') << std::setw(length) << "" << std::setfill(' ') << "\n";

    std::cout << "Open Weather calls: "  << std::setw(13) << open_weather_call_count << std::setw(20) << "[E] Enter Location" << "[S] Save Location" << std::endl;
    std::cout << std::left << "Sunrise Sunset calls: " << std::setw(11) << sunrise_sunset_call_count << std::setw(20) << "[R] Random Location" << "[L] Load Location" << std::endl;
    std::cout << std::left << "USNO calls: " << std::setw(21) << USNO_call_count << std::setw(20) << "[U] User Location" << "[P] Pause" << std::endl;
    std::cout << std::left << "GeoPlugin calls: " << std::setw(16) << geoPLUGIN_call_count << std::setw(20) << "[M] Main Menu" << "[Q] Quit" << std::endl;
}

void Weather::setCity(std::string city_input, std::string country_code_input) {

    whiteSpaceURLManager(city_input);
    city = city_input;
    country_code = country_code_input;
}

void Weather::loadCitiesList(){
    std::ifstream file("cities.json");
    if (!file) {
        std::cerr << "cities.json not found";
    } else {
        std::string str((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
        cities.Parse(str.c_str());
        //std::cout << "Cities list sucessfully loaded\n";
    }
}

time_t Weather::convertStringToTime(const std::string& str) {
    int current_year = getYear(time(0));
    if (str.substr(0, 4) != std::to_string(current_year)){
        return 0;
    }
    std::stringstream ss(str);
    std::tm time = {};
    ss >> std::get_time(&time, "%Y-%m-%dT%H:%M:%S");
    time_t time_seconds = _mkgmtime(&time);

    return time_seconds;
}

std::string Weather::convertUTCtimeToString(const int& previous_time) {
    bool is_negative = (previous_time < 0);
    int time_abs = std::abs(previous_time);
    int hours = time_abs / 3600;
    int minutes = (time_abs % 3600) / 60;

    std::ostringstream oss;
    oss << (is_negative ? "-" : "+")
        << std::setfill('0') << std::setw(2) << hours
        << ":" << std::setfill('0') << std::setw(2) << minutes;
    return oss.str();
}

std::string Weather::convertTimeToLocale(const time_t& previous_time) {
    if (previous_time == 0)
        return "00:00:00";
    else {
        char time_str[20];
        time_t previous_time_utc = (time_t)previous_time;
        previous_time_utc += timezone;
        struct tm local_timeinfo;
        gmtime_s(&local_timeinfo, &previous_time_utc);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", &local_timeinfo);
        std::string result(time_str);

        return result;
    }
}

struct tm Weather::timeToStructTM(time_t t) {
    struct tm timeinfo;
    gmtime_s(&timeinfo, &t);
    return timeinfo;
}

void Weather::setWeather() {
    if (doc.HasMember("weather") && doc["weather"].IsArray() && !doc["weather"].Empty()) {
        rapidjson::Value& weather_array = doc["weather"];
        if (weather_array[0].HasMember("main") && weather_array[0]["main"].IsString()) {
            weather_main = weather_array[0]["main"].GetString();
            for (auto& c : weather_main) { c = tolower(c); }
        }
        if (weather_array[0].HasMember("description") && weather_array[0]["description"].IsString()) {
            weather_description = weather_array[0]["description"].GetString();
        }
    } else
        std::cout << "ERROR: No conexion stablished\n";
}

std::string Weather::getStringData(const std::string& data) {
    std::string result{ "none" };
    if (doc.HasMember(data.c_str()) && doc[data.c_str()].IsString()) {
        result = doc[data.c_str()].GetString();
    }
    else {
        std::cerr << "ERROR: No " << data << " was found\n";
    }
    return result;
}

std::string Weather::getStringSubData(const std::string& main, const std::string& subdata) {
    std::string result{ "none" };
    if (doc.HasMember(main.c_str()) && doc[main.c_str()].IsObject()) {
        rapidjson::Value& main_obj = doc[main.c_str()];

        if (main_obj.HasMember(subdata.c_str()) && main_obj[subdata.c_str()].IsString()) {
            result = main_obj[subdata.c_str()].GetString();
        }
        else {
            std::cerr << "ERROR: No " << subdata << " was found\n";
        }
    }
    else {
        std::cerr << "ERROR: No " << subdata << " was found\n";
    }
    return result;
}

int Weather::getIntData(const std::string& input) {
    int result{ 0 };
    if (doc.HasMember(input.c_str()) && doc[input.c_str()].IsInt())
        result = doc[input.c_str()].GetInt();
    else {
        std::cerr << "ERROR: No " << input << " was found\n";
    }
    return result;
}

int Weather::getIntSubData(const std::string& main, const std::string& subdata) {

    int data{ 0 };

    if (doc.HasMember(main.c_str()) && doc[main.c_str()].IsObject()) {
        rapidjson::Value& obj = doc[main.c_str()];
        if (obj.HasMember(subdata.c_str()) && obj[subdata.c_str()].IsInt()) {
            data = (obj[subdata.c_str()].GetInt());
        }
        else { std::cerr << "ERROR: No " << subdata << " was found\n"; }
    }
    else { std::cerr << "ERROR: No " << main << " was found\n"; }
    return data;
}

double Weather::getDoubleData(const std::string& input) {

    double result{ 0 };

    if (doc.HasMember(input.c_str()) && doc[input.c_str()].IsDouble())
        result = doc[input.c_str()].GetDouble();
    else if (doc.HasMember(input.c_str()) && doc[input.c_str()].IsInt()) {
        result = static_cast<double>((doc[input.c_str()].GetInt()));
    }
    else {
        std::cerr << "ERROR: No " << input << " was found\n";
    }
    return result;
}

double Weather::getDoubleSubData(const std::string& main, const std::string& subdata) {

    double data{ 0.0 };
    if (doc.HasMember(main.c_str()) && doc[main.c_str()].IsObject()) {
        rapidjson::Value& obj = doc[main.c_str()];
        if (obj.HasMember(subdata.c_str()) && obj[subdata.c_str()].IsDouble()) {
            data = (obj[subdata.c_str()].GetDouble());
        }
        else if (obj.HasMember(subdata.c_str()) && obj[subdata.c_str()].IsInt()) {
            data = static_cast<double>((obj[subdata.c_str()].GetInt()));
        }
        else { std::cerr << "ERROR: No " << subdata << " was found\n"; }
    }
    return data;
}

std::string Weather::getDayLightStatus(const time_t& current_time) {

    if (astronomical_dawn.utc_sec == 0 && nautical_dawn.utc_sec == 0 && civil_dawn.utc_sec == 0 && sunrise.utc_sec == 0) {
        int month = getMonth(time(0));
        if (((month >= 4 && month <= 9) && lat < 0 || (month <= 3 && month >= 10)) && lat > 0) {
            status = DaylightStatus::Night;
            return "Nigth";
        }
        else if ((month >= 4 && month <= 9 && lat > 0) || (month <= 3 && month >= 10 && lat < 0)) {
            if (current_time < solar_noon.utc_sec) {
                status = DaylightStatus::Morning;
                return "Morning";
            }
            else if (current_time >= solar_noon.utc_sec) {
                status = DaylightStatus::Afternoon;
                return "Afternoon";
            }
        }
    }
    if (current_time < astronomical_dawn.utc_sec && current_time < solar_noon.utc_sec) {
        status = DaylightStatus::Night;
        return "Night";
    }
    else if ((current_time >= astronomical_dawn.utc_sec && current_time < nautical_dawn.utc_sec && nautical_dawn.utc_sec != 0 && current_time < solar_noon.utc_sec)
        || (current_time >= astronomical_dawn.utc_sec && current_time > nautical_dawn.utc_sec && nautical_dawn.utc_sec == 0 && sunrise.utc_sec == 0 && current_time < solar_noon.utc_sec)) {
        status = DaylightStatus::Astronomical_Dawn;
        return "Astronomical dawn";
    }
    else if ((current_time >= nautical_dawn.utc_sec && current_time < civil_dawn.utc_sec && civil_dawn.utc_sec != 0 && current_time < solar_noon.utc_sec)
        || (current_time >= nautical_dawn.utc_sec && current_time > civil_dawn.utc_sec && civil_dawn.utc_sec == 0 && sunrise.utc_sec == 0 && current_time < solar_noon.utc_sec)) {
        status = DaylightStatus::Nautical_Dawn;
        return "Nautical dawn";
    }
    else if ((current_time >= civil_dawn.utc_sec && current_time < sunrise.utc_sec && sunrise.utc_sec != 0)
        || (current_time >= civil_dawn.utc_sec && current_time > sunrise.utc_sec && sunrise.utc_sec == 0 && current_time < solar_noon.utc_sec)) {
        status = DaylightStatus::Civil_Dawn;
        return "Civil dawn";
    }
    else if (current_time >= sunrise.utc_sec && current_time < sunset.utc_sec && current_time < solar_noon.utc_sec) {
        status = DaylightStatus::Morning;
        return "Morning";
    }
    else if (current_time >= sunrise.utc_sec && current_time < sunset.utc_sec && current_time >= solar_noon.utc_sec) {
        status = DaylightStatus::Afternoon;
        return "Afternoon";
    }
    else if ((current_time >= sunset.utc_sec && current_time < civil_dusk.utc_sec)
        || (current_time >= sunset.utc_sec && civil_dusk.utc_sec == 0 && nautical_dusk.utc_sec == 0 && astronomical_dusk.utc_sec == 0)) {
        status = DaylightStatus::Civil_Dusk;
        return "Civil dusk";
    }
    else if ((current_time >= civil_dusk.utc_sec && current_time < nautical_dusk.utc_sec && nautical_dusk.utc_sec != 0)
        || (current_time >= civil_dusk.utc_sec && nautical_dusk.utc_sec == 0 && astronomical_dusk.utc_sec == 0)) {
        status = DaylightStatus::Nautical_Dusk;
        return "Nautical dusk";
    }
    else if ((current_time >= nautical_dusk.utc_sec && current_time < astronomical_dusk.utc_sec && astronomical_dusk.utc_sec != 0)
        || (current_time > nautical_dusk.utc_sec && astronomical_dusk.utc_sec == 0)) {
        status = DaylightStatus::Astronomical_Dusk;
        return "Astronomical dusk";
    }
    else if (current_time >= astronomical_dusk.utc_sec && astronomical_dusk.utc_sec != 0 || current_time < astronomical_dawn.utc_sec) {
        status = DaylightStatus::Night;
        return "Night";
    }
    else
        return "error un getting daylight status";
}

int Weather::getYear(time_t t) {

    struct tm timeinfo;
    gmtime_s(&timeinfo, &t);

    return timeinfo.tm_year + 1900;
}

int Weather::getMonth(time_t t) {

    struct tm timeinfo;
    gmtime_s(&timeinfo, &t);

    return timeinfo.tm_mon + 1;
}

int Weather::getDay(time_t t) {

    struct tm timeinfo;
    gmtime_s(&timeinfo, &t);

    return timeinfo.tm_mday;
}

int Weather::getHour(time_t t) {

    struct tm timeinfo;
    gmtime_s(&timeinfo, &t);

    return timeinfo.tm_hour;
}

int Weather::correct_day_offset() {

    return day_offset;
}

bool Weather::data_is_wrong(int& day_offset) {

    int day_length = 86400;
    time_t current_UTC_time = time(0);

    Time sunrise_time(getStringSubData("results", "sunrise"), *this);

    if ((sunrise_time.utc_sec - current_UTC_time) > day_length) {
        day_offset = -1;
        return true;
    }
    else if (((sunrise_time.utc_sec - current_UTC_time) < -day_length) && sunrise.utc_sec != 0) {
        day_offset = 1;
        return true;
    }
    else {
        day_offset = 0;
        return false;
    }
}

void Weather::whiteSpaceURLManager(std::string& input) {

    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == ' ') {
            input.replace(i, 1, "%20");
            i += 2;
        }
    }
}

Weather::Time::Time(const std::string& time, Weather& weather)
    : utc_sec{ weather.convertStringToTime(time) }, local_str{ weather.convertTimeToLocale(utc_sec) }, info{ weather.timeToStructTM(utc_sec) } {}

Weather::Time::Time(const int& time, Weather& weather)
    : utc_sec{ time }, local_str{ weather.convertTimeToLocale(time) }, info{ weather.timeToStructTM(time) } {}


