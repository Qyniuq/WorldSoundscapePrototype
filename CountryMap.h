#pragma once
#include <map>
#include <string>

std::map<std::string, std::string> createCountryMap();
std::string getCountryName(const std::map<std::string, std::string>&, const std::string&);
