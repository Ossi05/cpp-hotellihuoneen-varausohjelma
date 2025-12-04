#include "HotelApp.h"
#include <clocale>
#include "utils.h"
#include <string>

int main() {

	// Use Finnish locale for formatting
	std::setlocale(LC_ALL, "fi_FI");

	// Setup Hotel Application
	std::string config_file_name{ "config.cfg" };
	HotelApp app{ config_file_name };
	app.run();

	return 0;
}