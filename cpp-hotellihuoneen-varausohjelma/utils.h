#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <sstream>
#include <vector>

/*
	Asks user for input and returns it

	Params:
		msg: message to display to the usger
		error_msg: message to display on invalid input
		allow_empty: if empty input is allowed (default: false)
		validator: function to validate input (default: nullptr)
*/
template <typename T>
T get_input(
	const std::string& msg,
	const std::string& error_msg = "Virheellinen syöte.",
	const bool allow_empty = false,
	const std::function<bool(T&)> validator = nullptr
) {
	T value{};
	while (true) {
		std::cout << msg;

		std::string input;
		std::getline(std::cin, input);
		std::stringstream ss{ input };

		if (allow_empty && input.empty()) {
			// Sallitaan tyhjä ja input oli tyhjä
			break;
		}

		if ((ss >> value) && (!validator || validator(value))) break; // Annettiin oikea input. Poistutaan silmukata

		// Annettiin väärä input
		if (error_msg != "") {
			std::cout << error_msg << std::endl;
			continue;
		}
		std::cout << "Virheellinen syote\n";
	}

	return value;
}

void clear_screen();
void wait_for_input();
int get_random_number(int min, int max);
void print_line(char c, int width);
std::string to_lower(const std::string& str);
void save_to_csv(const std::string& text, const std::string& filename);
void clear_file(const std::string& filename);
std::vector<std::string> get_csv_rows(const std::string& filename);
void trim(std::string& str);
std::string two_column_text(
	int print_width,
	const std::string& left_text,
	int padding_left,
	std::string separator = "",
	const std::string& right_text = "",
	int padding_right = 0
);