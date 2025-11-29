#include "Config.h"
#include <fstream>
#include <sstream>
#include "exceptions.hpp"
#include <iostream>
#include "utils.h"

void Config::load_config()
{
	std::ifstream file{ config_file_name };
	if (!file) {
		throw FileAccessException("Config tiedoston avaaminen epäonnistui: " + config_file_name);
	}

	std::string line{};
	while (std::getline(file, line)) {
		std::istringstream iss{ line };
		std::string key{};
		if (std::getline(iss, key, '=')) { // Avain ja arvo erotetaan = merkillä
			std::string value{};
			if (std::getline(iss, value)) {
				// Poistetaan ylimääräiset välilyönnit
				trim(key);
				trim(value);
				config_map[key] = value;
			}
		}
	}
}

Config::Config(const std::string& config_file_name) :
	config_file_name{ config_file_name }
{
	load_config();
}

std::string Config::get_value(const std::string& key) const
{
	try {
		return config_map.at(key); // Palautetaan arvo, jos avain löytyy
	}
	catch (const std::out_of_range) {
		return ""; // Avainta ei löytynyt
	}
}

void Config::set_value(const std::string& key, const std::string& value)
{
	config_map[key] = value;
	save_config();
}


void Config::save_config() const
{
	std::ofstream file{ config_file_name };
	if (!file) {
		throw FileAccessException("Config tiedoston avaaminen epäonnistui: " + config_file_name);
	}
	for (const auto& pair : config_map) {
		file << pair.first << "=" << pair.second << std::endl;
	}
}