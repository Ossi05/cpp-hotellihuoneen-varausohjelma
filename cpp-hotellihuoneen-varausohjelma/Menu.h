#pragma once
#include <vector>
#include "MenuOption.h"

class Menu
{	
private:
	void print_title(const std::string title) const;
	std::vector<MenuOption> menu_options;

public:
	const static int print_width;

	Menu(const std::vector<MenuOption>& options);
	void print_menu_options() const;
	int get_menu_choice() const;
	void handle_choice(int choice) const;
};
