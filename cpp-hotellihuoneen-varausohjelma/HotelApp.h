#pragma once
#include "Hotel.h"
#include <vector>
#include <string>
#include "MenuOption.h"
#include "Menu.h"
#include "Config.h"
#include "CSVReservationHandler.h"

class HotelApp
{
public:
	HotelApp(const std::string& config_file_name);
	void run();
private:

	Config config;
	Menu menu;
	CSVReservationHandler csv_reservation_handler;
	Hotel hotel;
	bool has_csv_file;
	bool is_running;


	Hotel loadHotelFromConfig();
	void load_reservations_from_csv();
	int get_num_rooms_from_config();
	std::vector<double> get_sale_percentages_from_config() const;

	// Valikon toiminnot
	void handle_exit_program();

	// Varaukset
	void create_reservation();
	void show_reservations() const;
	void find_reservation_by_id() const;
	void find_reservations_by_name() const;
	void remove_reservation();
	void print_info() const;

};