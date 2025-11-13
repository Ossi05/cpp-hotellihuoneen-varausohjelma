#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <sstream>

/*
	Kysyy käyttäjältä inputin ja palauttaa sen

	@param msg          Viesti, joka näytetään käyttäjälle syötettä pyydettäessä.
	@param error_msg    Viesti, joka näytetään, jos käyttäjä antaa virheellisen syötteen (oletus: "Virheellinen syöte.").
	@param allow_empty  Jos true, tyhjä syöte hyväksytään.
	@param validator    Valinnainen funktio, jolla voidaan tarkistaa syötteen kelpoisuus. Palauttaa true, jos syöte on hyväksyttävä.

	@return Käyttäjän syöttämä arvo tyyppiä T.
*/
template <typename T>
T get_input(const std::string& msg, const std::string& error_msg = "Virheellinen syöte.", const bool allow_empty = false, const std::function<bool(T&)> validator = nullptr) {

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