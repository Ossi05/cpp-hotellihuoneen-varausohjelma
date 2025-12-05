#include "HotelApp.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "utils.h"
#include <string>
#include "MenuOption.h"
#include "Room.h"
#include "exceptions.hpp"

/*
	Initialize Hotel app
*/
HotelApp::HotelApp(const std::string& config_file_name) :
	config{ config_file_name },
	csv_reservation_handler{ config.get_value("CSV_FILE_NAME") },
	hotel{ loadHotelFromConfig() },
	is_running{ false },
	menu{ { // Menu options
		{ "Luo varaus", [this]() { create_reservation(); } },
		{ "Näytä kaikki varaukset", [this]() { show_reservations(); } },
		{ "Hae varaus varausnumerolla", [this]() { find_reservation_by_id(); } },
		{ "Hae varauksia varaajan nimellä", [this]() { find_reservations_by_name(); } },
		{ "Poista varaus", [this]() { remove_reservation(); } },
		{ "Poistu ohjelmasta", [this]() { handle_exit_program(); }},
	} }
{
}

/*
	HotelApp's main program loop
*/
void HotelApp::run()
{
	is_running = true;

	// If we have csv, try to load reservations from it
	if (csv_reservation_handler.has_file_name()) {
		is_running = try_load_reservations_from_csv();
	}

	while (is_running)
	{
		try {
			// 1. Clear screen and print hotel
			clear_screen();
			std::cout << hotel;

			// 2. Print menu and get choice
			menu.print_menu_options();
			int choice{ menu.get_menu_choice() };

			// 3. Handle choice
			clear_screen();
			menu.handle_choice(choice - 1);
			std::cout << std::endl;
		}
		catch (const std::exception& e) {
			// Catch standard exceptions
			clear_screen();
			std::cerr << "Tapahtui virhe: " << e.what() << std::endl;
		}
		catch (...) {
			// Catch unexpected errors
			clear_screen();
			std::cerr << "Tapahtui tuntematon virhe." << std::endl;
		}

		// Wait for user input before continuing
		wait_for_input();
	}
}

/*
	Create a new reservation
	1. Check for available rooms
	2. Get reservation details from user
	3. Create reservation
	4. Print reservation details
*/
void HotelApp::create_reservation()
{
	// 1. Check for available rooms
	if (hotel.get_num_rooms_available() == 0) {
		std::cerr << "Huoneita ei ole vapaana";
		return;
	}
	if (!hotel.has_available_reservation_ids()) {
		std::cerr << "Ei vapaita varausnumeroita";
		return;
	}

	// 2. Get reservation details from user
	// Print room types
	std::cout << "Valitse huoneentyypi:\n";
	for (const auto& room_data : Room::ROOM_TYPE_DATA) {
		RoomType room_type = room_data.first;
		size_t num_available{ hotel.get_num_rooms_available(room_type) };
		std::cout
			<< (static_cast<int>(room_type) + 1) << ": "
			<< room_data.second.name << " (" << num_available << " kpl)" << std::endl;
	}
	std::cout << std::endl;

	// Get room type from user
	RoomType room_type{};
	while (true) {
		int input{ get_input<int>("Huoneentyyppi: ", "Virheellinen syöte", false,
			[](int num) {return num >= 1 && num <= Room::ROOM_TYPE_DATA.size(); }) - 1 };

		room_type = static_cast<RoomType>(input);
		size_t num_rooms_available{ hotel.get_num_rooms_available(room_type) };
		if (num_rooms_available != 0) {
			// Available room found
			break;
		}
		std::cout << "Huoneita ei vapaana." << std::endl;
	};

	// Get guest name and number of nights
	std::string guest_name{ get_input<std::string>("Varaajan nimi: ") };
	int num_nights{ get_input<int>(
		"Montako yötä?: ",
		"Öitä pitää olla vähintää yksi",
		false,
		[](int n) {return n >= 1; })
	};

	// Get room
	auto room{ hotel.get_available_room(room_type) };
	int room_number{ room->get_room_number() };

	// 3. Create reservation
	auto reservation{
		hotel.create_reservation(room_number, guest_name, num_nights)
	};
	if (csv_reservation_handler.has_file_name()) { // Save reservation to CSV
		csv_reservation_handler.save_reservation(reservation);
	}

	// 4. Print reservation details
	std::cout << std::endl;
	std::cout << "Huone: " << room_number << " varattu " << std::endl;
	std::cout
		<< "Hinta " << reservation->get_total_price() << " euroa ("
		<< reservation->get_sale_percentage() << "% alennus)" << std::endl;
}

/*
	Show Reservations
*/
void HotelApp::show_reservations() const { hotel.list_reservations(); }

/*
	Find Reservations by ID
*/
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


/*
	Find Reservations by Name
	1. Get guest name from user
	2. Search reservations by guest name
	3. Print results
*/
void HotelApp::find_reservations_by_name() const
{
	// 1. Get guest name from user
	std::string guest_name{};
	std::cout << "Anna varaajan nimi: ";
	std::getline(std::cin, guest_name);

	// 2. Search reservations by guest name
	auto reservations{ hotel.get_reservations_by_guest_name(guest_name) };
	if (reservations.size() == 0) {
		std::cout << "Varauksia ei löytynyt" << std::endl;
		return;
	}

	// 3. Print results
	std::cout << std::endl << "Tulokset:\n" << std::endl;
	for (const auto& reservation : reservations) {
		std::cout << *reservation << std::endl;
	}
}

/*
	Remove Reservation
*/
void HotelApp::remove_reservation()
{
	int reservation_id{ get_input<int>("Anna varausnumero: ") };
	try {
		hotel.remove_reservation(reservation_id);
		if (csv_reservation_handler.has_file_name()) {
			csv_reservation_handler.remove_reservation(reservation_id); // Poistetaan varaus CSV-tiedostosta
		}
		std::cout << "Varaus (" << reservation_id << ") poistettu" << std::endl;
	}
	catch (const ReservationNotFoundException&) {
		std::cerr << "Varausta ei löytynyt" << std::endl;
	}

}

/*
	Print details about the hotel
*/
void HotelApp::print_info() const
{
	std::cout << "Tiedostoon tallennus: "
		<< (csv_reservation_handler.has_file_name() ? "Kyllä (" + csv_reservation_handler.get_file_name() + ")" : "Ei") << std::endl;
	std::cout << "Hotellin nimi: " << hotel.get_name() << std::endl << std::endl;
	std::cout
		<< "Varaus ID (min "
		<< hotel.get_min_reservation_id() << ", max " << hotel.get_max_reservation_id()
		<< ")"
		<< std::endl;
	auto sale_percentages{ hotel.get_sale_percentages() };
	std::cout << "Mahdolliset aleprosentit: ";
	std::cout << std::fixed << std::setprecision(2);
	std::cout << sale_percentages.at(0);
	for (size_t i = 1; i < sale_percentages.size(); ++i) {
		std::cout << ", " << sale_percentages.at(i);
	}
	std::cout << "%" << std::endl;
}

/*
	Handle exiting the program
*/
void HotelApp::handle_exit_program()
{
	std::cout << "Poistutaan ohjelmasta..." << std::endl;
	is_running = false;
	if (!csv_reservation_handler.has_file_name()) { return; }

	// Save all reservations to CSV
	csv_reservation_handler.save_all(hotel.get_all_reservations());
	std::cout
		<< "\nVaraukset tallennettu tiedostoon "
		<< csv_reservation_handler.get_file_name()
		<< std::endl;
}

/*
	Load hotel from config file
*/
Hotel HotelApp::loadHotelFromConfig()
{
	std::string hotel_name{ config.get_value("HOTEL_NAME") };
	int num_rooms{ get_num_rooms_from_config() };
	int min_reservation_id{ std::stoi(config.get_value("MIN_RESERVATION_ID")) };
	int max_reservation_id{ std::stoi(config.get_value("MAX_RESERVATION_ID")) };
	std::vector<double> sale_percentages{ get_sale_percentages_from_config() };

	return Hotel{
		hotel_name,
		num_rooms,
		sale_percentages,
		min_reservation_id,
		max_reservation_id
	};

}

/*
	Try to load reservations from a CSV file
*/
bool HotelApp::try_load_reservations_from_csv()
{
	try {
		auto reservations{ csv_reservation_handler.load_reservations() };
		for (const auto& reservation : reservations) {
			// Yhdistetään varaukset huoneisiin
			hotel.create_reservation(
				reservation->get_room_number(),
				reservation->get_guest_name(),
				reservation->get_num_nights()
			);
		}
		return true;
	}
	catch (const ReservationIdAlreadyExistsException& e) {
		std::cerr << "CSV-tiedoston latauksessa tapahtui virhe: " << e.what() << std::endl;
	}
	catch (const RoomNotFoundException& e) {
		std::cerr << "CSV-tiedoston latauksessa tapahtui virhe: " << e.what() << std::endl;
	}
	catch (const RoomNotAvailableException& e) {
		std::cerr << "CSV-tiedoston latauksessa tapahtui virhe: " << e.what() << std::endl;
	}
	catch (const NoAvailableReservationIdsException& e) {
		std::cerr << "CSV-tiedoston latauksessa tapahtui virhe: " << e.what() << std::endl;
	}
	return false;
}

/*
	Get sale percentages from a config file
*/
std::vector<double> HotelApp::get_sale_percentages_from_config() const
{
	std::vector<double> sale_percentages{};
	std::string sale_percentages_str{ config.get_value("SALE_PERCENTAGES") };
	std::istringstream ss{ sale_percentages_str };

	double sale_percentage{};
	while (ss >> sale_percentage) {
		sale_percentages.push_back(sale_percentage);
	}

	if (sale_percentages.empty()) {
		sale_percentages.emplace_back(0);
	}

	return sale_percentages;
}

/*
	Get number of rooms from config file
	If TOTAL_ROOM_AMT key is not found,
	random number between MIN_ROOMS_AMT and MAX_ROOMS_AMT is generated
*/
int HotelApp::get_num_rooms_from_config()
{
	std::string total_room_amt_str{ config.get_value("TOTAL_ROOM_AMT") };
	int num_rooms{};

	if (!total_room_amt_str.empty()) {
		// TOTAL_ROOM_AMT found in config
		num_rooms = std::stoi(total_room_amt_str);
	}
	else {
		// Generate random number of rooms between MIN_ROOMS_AMT and MAX_ROOMS_AMT
		int min_rooms_amt{ std::stoi(config.get_value("MIN_ROOMS_AMT")) };
		int max_rooms_amt{ std::stoi(config.get_value("MAX_ROOMS_AMT")) };

		if (!min_rooms_amt || !max_rooms_amt) {
			throw ConfigKeyNotFoundException{ "MIN_ROOMS_AMT tai MAX_ROOMS_AMT avainta ei löytynyt" };
		}
		num_rooms = get_random_number(min_rooms_amt, max_rooms_amt);

		// Making rooms divisible by 10, so it looks better
		num_rooms =
			num_rooms % 10 == 0 ?
			num_rooms :
			num_rooms - (num_rooms % 10);
		config.set_value("TOTAL_ROOM_AMT", std::to_string(num_rooms));
	}
	return num_rooms;
}


