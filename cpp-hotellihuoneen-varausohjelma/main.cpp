#include "HotelApp.h"
#include <clocale>
#include "utils.h"
#include <string>

int main() {

	std::string hotel_name{ "C++ Hotelli" };
	std::string csv_file_name{ "testi.csv" }; // Tiedosto, johon varaukset tallennetaan
	int min_rooms_amt{ 40 }; // Minimi m‰‰r‰ huoneita
	int max_rooms_amt{ 300 }; // Maksimi m‰‰r‰ huoneita


	int room_amt{ get_random_number(min_rooms_amt, max_rooms_amt) };
	// Tehd‰‰n huonem‰‰r‰ jaolliseksi kymmenell‰
	room_amt =
		room_amt % 10 == 0 ?
		room_amt :
		room_amt - (room_amt % 10);

	std::setlocale(LC_ALL, "fi_FI");
	HotelApp app{ hotel_name, room_amt, csv_file_name };
	app.run();
	return 0;
}