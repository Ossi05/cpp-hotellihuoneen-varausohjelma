#include "Menu.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include "MenuOption.h"
#include "utils.h"


const int Menu::print_width{ 35 };

Menu::Menu(const std::vector<MenuOption>& options)
	: menu_options{ options }
{
}

void Menu::print_menu_options() const
{
	for (size_t i = 0; i < menu_options.size(); i++)
	{
		std::cout << i + 1 << " " << menu_options.at(i).name << std::endl;
	}
	std::cout << std::setfill('-') << std::setw(Menu::print_width) << "" << std::setfill(' ') << std::endl;
}

int Menu::get_menu_choice() const
{
	const std::string msg{ "Valitse toiminto (1-" + std::to_string(menu_options.size()) + "): " };
	const int input = get_input<int>(
		msg,
		"Tuntematon komento",
		false,
		[this](int& value) { return value >= 1 && value <= static_cast<int>(menu_options.size()); }
	);
	return input;
}

void Menu::handle_choice(int choice) const
{
	// TODO VIRHEENKÄSITTELY

	// Kutsutaan valittua toimintoa
	const MenuOption& option{ menu_options.at(choice) };
	print_title(option.name);
	option.action();
}

void Menu::print_title(const std::string title) const
{
	print_line('=', Menu::print_width);
	std::cout << std::setw((Menu::print_width + title.length()) / 2) << title << std::endl;
	print_line('=', Menu::print_width);
}