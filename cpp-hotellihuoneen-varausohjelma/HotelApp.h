#pragma once
#include "Hotel.h"
#include <vector>
#include <string>
#include "MenuOption.h"
#include "Menu.h"
#include "Config.h"
#include "CSVReservationHandler.h"

/*
	HotelApp class manages the main program
*/
class HotelApp
{
private:
	Config config;
	Menu menu;
	CSVReservationHandler csv_reservation_handler;
	Hotel hotel;
	bool is_running;

	// Menu actions
	void handle_exit_program();

	// Reservations
	void create_reservation();
	void show_reservations() const;
	void find_reservation_by_id() const;
	void find_reservations_by_name() const;
	void remove_reservation();
	void print_info() const;

	// Helper functions for loading hotel and reservations from config and csv
	Hotel loadHotelFromConfig();
	bool try_load_reservations_from_csv();
	int get_num_rooms_from_config();
	std::vector<double> get_sale_percentages_from_config() const;
public:
	HotelApp(const std::string& config_file_name);
	void run();
};