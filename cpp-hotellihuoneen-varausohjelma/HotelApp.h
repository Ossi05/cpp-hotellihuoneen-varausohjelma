#pragma once
#include "Hotel.h"
#include <vector>
#include <string>
#include "MenuOption.h"
#include "Menu.h"
#include "Config.h"

class HotelApp
{
public:
	HotelApp(const std::string& config_file_name);
	void run();
private:

	Config config;
	Menu menu;
	Hotel hotel;
	bool is_running;

	Hotel loadHotelFromConfig();

	// Valikon toiminnot
	void handle_exit_program();

	// Varaukset
	void create_reservation();
	void show_reservations() const;
	void find_reservation_by_id() const;
	void find_reservations_by_name() const;
	void remove_reservation();

};