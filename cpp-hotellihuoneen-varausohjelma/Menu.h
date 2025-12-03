#pragma once
#include <vector>
#include <string>
#include "MenuOption.h"

class Menu
{
private:
	void print_title(const std::string title) const;
	std::vector<MenuOption> menu_options;

public:
	const static int print_width;
	const static void print_two_col_text(const std::string& text_left, const std::string& text_right = "");

	Menu(const std::vector<MenuOption>& options);
	void print_menu_options() const;
	int get_menu_choice() const;
	void handle_choice(int choice) const;
};
