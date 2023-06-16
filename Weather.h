#pragma once
#include <iostream>
#include <string>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <map>

class Weather
{
private: 
    std::ifstream in_file;
    CURL* curl;
    CURLcode res;
    std::string api_key;
    std::string URL;
    rapidjson::Document doc;

    struct MemoryStruct {
        char* memory;
        size_t size;
    };

    struct Time {
        time_t utc_sec;
        std::string local_str;
        struct tm info;

        Time(const std::string& time, Weather& weather);

        Time(const int& time, Weather& weather);
    };

    struct MemoryStruct chunk;

    int open_weather_call_count;
    int sunrise_sunset_call_count;
    int USNO_call_count;
    int geoPLUGIN_call_count;
    bool data_wrong;
    int day_offset;

public:
    bool No_Conexion {false};
    bool No_ApiCode{ false };
    bool No_Saved_Locations{ false };
    bool No_Location_Found{ false };

    rapidjson::Document cities;
    std::string city;
    std::string state_code;
    std::string country_code;
    std::string country_name {"none"};
    std::map<std::string, std::string> countryMap;
    bool user_location;
    enum class APIs { OpenWeather = 0, SunriseAndSunset = 1, USNO = 2, geoPLUGIN = 3 };
    enum class DaylightStatus { Night = 0, Astronomical_Dawn, Nautical_Dawn, Civil_Dawn, Morning = 4, Afternoon, Civil_Dusk, Nautical_Dusk, Astronomical_Dusk };
    std::string scale;
    std::string tonality;
    std::vector<std::string> scale_note_names;
    int points;

    std::string city_input;
    double lon;
    double lat;
    std::string weather_main;
    std::string weather_description;
    double temperature;
    double feels_like;
    double wind_speed;
    int pressure;
    double rain_vol;
    double snow_vol;
    int cloudiness;
    int humidity;
    int visibility;

    Time current_time;
    Time sunrise;
    Time sunset;
    Time solar_noon;
    Time civil_dawn;
    Time nautical_dawn;
    Time astronomical_dawn;
    Time civil_dusk;
    Time nautical_dusk;
    Time astronomical_dusk;

    int timezone;
    time_t current_time_seconds;
    time_t end_of_current_day;
    time_t end_of_UTC_day;
    time_t day_length_seconds;

    std::string time_offset;
    std::string city_time;
    std::string day_light_status;
    DaylightStatus status;
    std::string day_length;

    double moon_age;
    std::string moon_phase;
    std::string moon_fracillum;

public:

    Weather(bool user_location, std::string city, std::string country_code, std::string state_code_input);

    static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void callAPI(std::string URL, APIs api);
    void callAllAPIs();
    void setCity(std::string city, std::string country_code);
    void loadCitiesList();
    void setWeather();
    void updateGeoLocation();
    void updateOpenWeatherData();
    void updateSunsetAndSunriseData();
    void updateMoonPhase();
    int correct_day_offset();
    bool data_is_wrong(int& day_offset);
    std::string getDayLightStatus(const time_t& current_time);

    void display();

    int getIntData(const std::string& input);
    int getIntSubData(const std::string& main, const std::string& subdata);
    double getDoubleData(const std::string& input);
    double getDoubleSubData(const std::string& main, const std::string& subata);
    std::string getStringData(const std::string& data);
    std::string getStringSubData(const std::string& main, const std::string& subata);

    time_t convertStringToTime(const std::string& str);
    std::string convertTimeToLocale(const time_t& time_str_size);
    std::string convertUTCtimeToString(const int& previous_time);
    struct tm timeToStructTM(time_t t);

    int getYear(time_t t);
    int getMonth(time_t t);
    int getDay(time_t t);
    int getHour(time_t t);
    time_t getEndOfCurrentDay();
    time_t getEndOfUTCDay();

    void whiteSpaceURLManager(std::string& input);
};
