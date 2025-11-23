#include "HotelApp.h"
#include <clocale>


int main() {

	std::setlocale(LC_ALL, "fi_FI");
	HotelApp app{ "C++ hotelli", 20, "testi.csv"};
	app.run();
	return 0;
}