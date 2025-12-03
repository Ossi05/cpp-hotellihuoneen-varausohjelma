#include "HotelApp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "utils.h"
#include <string>
#include "MenuOption.h"
#include "Room.h"
#include "exceptions.hpp"


Hotel HotelApp::loadHotelFromConfig()
{
	std::string hotel_name{ config.get_value("HOTEL_NAME") };
	std::string total_room_amt_str{ config.get_value("TOTAL_ROOM_AMT") };
	std::string csv_file_name = config.get_value("CSV_FILE_NAME");

	int num_rooms{};
	if (!total_room_amt_str.empty()) {
		num_rooms = std::stoi(total_room_amt_str);
		config.set_value("TOTAL_ROOM_AMT", std::to_string(num_rooms));
	}
	else {
		// Arvotaan huonemäärä, jos avainta ei löydy
		int min_rooms_amt{ std::stoi(config.get_value("MIN_ROOMS_AMT")) };
		int max_rooms_amt{ std::stoi(config.get_value("MAX_ROOMS_AMT")) };

		if (!min_rooms_amt || !max_rooms_amt) {
			throw ConfigKeyNotFoundException{ "MIN_ROOMS_AMT tai MAX_ROOMS_AMT avainta ei löytynyt" };
		}
		num_rooms = get_random_number(min_rooms_amt, max_rooms_amt);

		// Tehdään huonemäärä jaolliseksi kymmenellä, jotta se näyttää selkeämmältä
		num_rooms =
			num_rooms % 10 == 0 ?
			num_rooms :
			num_rooms - (num_rooms % 10);
	}

	return Hotel{ hotel_name, num_rooms, csv_file_name };
}


HotelApp::HotelApp(const std::string& config_file_name) :
	config{ config_file_name },
	hotel{ loadHotelFromConfig() },
	is_running{ true },
	menu{ {
		{ "Luo varaus", [this]() { create_reservation(); } },
		{ "Näytä kaikki varaukset", [this]() { show_reservations(); } },
		{ "Hae varaus varausnumerolla", [this]() { find_reservation_by_id(); } },
		{ "Hae varauksia varaajan nimellä", [this]() { find_reservations_by_name(); } },
		{ "Poista varaus", [this]() { remove_reservation(); } },
		{ "Poistu ohjelmasta", [this]() { handle_exit_program(); }}
	} }
{
}


/*
Hotellisovelluksen pääohjelma, joka suoritetaan alussa
*/
void HotelApp::run()
{
	try {
		hotel.load_reservations_from_csv();
	}
	catch (const ReservationIdAlreadyExistsException& e) {
		std::cerr << "CSV-tiedoston latauksessa tapahtui virhe: " << e.what() << std::endl;
		wait_for_input();
	}
	catch (const RoomNotFoundException& e) {
		std::cerr << "CSV-tiedoston latauksessa tapahtui virhe: " << e.what() << std::endl;
		wait_for_input();
	}
	catch (const RoomNotAvailableException& e) {
		std::cerr << "CSV-tiedoston latauksessa tapahtui virhe: " << e.what() << std::endl;
		wait_for_input();
	}

	is_running = true;

	while (is_running)
	{
		try {
			clear_screen();
			std::cout << hotel;
			menu.print_menu_options();
			int choice{ menu.get_menu_choice() };
			clear_screen();
			menu.handle_choice(choice - 1);
			std::cout << std::endl;
		}
		catch (const std::exception& e) { // Virheitä ei pitäisi koskaan tapahtua
			clear_screen();
			std::cerr << "Tapahtui virhe: " << e.what() << std::endl;
		}
		catch (...) {
			clear_screen();
			std::cerr << "Tapahtui tuntematon virhe." << std::endl;
		}
		wait_for_input();
	}
}
/*
Valikon toiminnot
*/


void HotelApp::create_reservation()
{
	// 1. Tarkistetaan onko huoneita vapaana
	if (hotel.get_num_rooms_available() == 0) {
		std::cerr << "Huoneita ei ole vapaana";
		return;
	}

	// 2. Annetaan varauksen tiedot
	std::cout << "Valitse huoneentyypi:\n";
	for (const auto& room_data : Room::room_type_data) {
		RoomType room_type = room_data.first;
		size_t num_available{ hotel.get_num_rooms_available(room_type) };
		std::cout << (static_cast<int>(room_type) + 1) << ": " << room_data.second.name << " (" << num_available << " kpl)" << std::endl;
	}

	std::cout << std::endl;

	RoomType room_type{};
	while (true) {
		int input{ get_input<int>("Huoneentyyppi: ", "Virheellinen syöte", false,
			[](int num) {return num >= 1 && num <= Room::room_type_data.size(); }) - 1 };

		room_type = static_cast<RoomType>(input);
		size_t num_rooms_available{ hotel.get_num_rooms_available(room_type) };
		if (num_rooms_available != 0) {
			// Vapaa huone löytyi
			break;
		}
		std::cout << "Huoneita ei vapaana." << std::endl;
	};

	int room_number{};

	auto room{ hotel.get_available_room(room_type) };
	room_number = room->get_room_number();

	std::string guest_name{ get_input<std::string>("Varaajan nimi: ") };
	int num_nights{ get_input<int>("Montako yötä?: ", "Öitä pitää olla vähintää yksi", false, [](int n) {return n >= 1; }) };

	// 3. Luodaan varaus
	auto reservation{
		hotel.create_reservation(room_number, guest_name, num_nights)
	};
	std::cout << std::endl;
	std::cout << "Huone: " << room_number << " varattu " << std::endl;
	std::cout << "Hinta " << reservation->get_total_price() << " euroa (" << reservation->get_sale_percentage() << "% alennus)" << std::endl;
}

void HotelApp::show_reservations() const
{
	hotel.list_reservations();
}
void HotelApp::find_reservation_by_id() const
{
	int reservation_id{ get_input<int>("Anna varausnumero: ") };
	try {
		auto reservation{ hotel.get_reservation_by_id(reservation_id) };
		std::cout << std::endl << *reservation << std::endl;
	}
	catch (const ReservationNotFoundException&) {
		std::cerr << "Varausta ei löytynyt" << std::endl;
	}
}
void HotelApp::find_reservations_by_name() const
{
	std::string guest_name{};
	std::cout << "Anna varaajan nimi: ";
	std::getline(std::cin, guest_name);

	auto reservations{ hotel.get_reservations_by_guest_name(guest_name) };
	if (reservations.size() == 0) {
		std::cout << "Varauksia ei löytynyt" << std::endl;
		return;
	}

	std::cout << std::endl << "Tulokset:\n" << std::endl;
	for (const auto& reservation : reservations) {
		std::cout << *reservation << std::endl;
	}
}
void HotelApp::remove_reservation()
{
	int reservation_id{ get_input<int>("Anna varausnumero: ") };
	try {
		hotel.remove_reservation(reservation_id);
		std::cout << "Varaus (" << reservation_id << ") poistettu" << std::endl;
	}
	catch (const ReservationNotFoundException&) {
		std::cerr << "Varausta ei löytynyt" << std::endl;
	}

}

void HotelApp::handle_exit_program()
{
	std::cout << "Poistutaan ohjelmasta..." << std::endl;
	std::string csv_file_name{ hotel.get_csv_file_name() };
	if (!csv_file_name.empty()) {
		hotel.save_reservations_to_csv();
		std::cout << "\nVaraukset tallennettu tiedostoon " << csv_file_name << std::endl;
	}

	is_running = false;
}

