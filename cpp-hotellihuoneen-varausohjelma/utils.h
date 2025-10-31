#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <sstream>

/*
	Kysyy k�ytt�j�lt� inputin ja palauttaa sen

	@param msg          Viesti, joka n�ytet��n k�ytt�j�lle sy�tett� pyydett�ess�.
	@param error_msg    Viesti, joka n�ytet��n, jos k�ytt�j� antaa virheellisen sy�tteen (oletus: "Virheellinen sy�te.").
	@param allow_empty  Jos true, tyhj� sy�te hyv�ksyt��n.
	@param validator    Valinnainen funktio, jolla voidaan tarkistaa sy�tteen kelpoisuus. Palauttaa true, jos sy�te on hyv�ksytt�v�.

	@return K�ytt�j�n sy�tt�m� arvo tyyppi� T.
*/
template <typename T>
T get_input(const std::string& msg, const std::string& error_msg = "Virheellinen sy�te.", const bool allow_empty = false, const std::function<bool(T&)> validator = nullptr) {

	T value{};
	while (true) {
		std::cout << msg;

		std::string input;
		std::getline(std::cin, input);
		std::stringstream ss{ input };

		if (allow_empty && input.empty()) {
			// Sallitaan tyhj� ja input oli tyhj�
			break;
		}

		if ((ss >> value) && (!validator || validator(value))) break; // Annettiin oikea input. Poistutaan silmukata

		// Annettiin v��r� input
		if (error_msg != "") {
			std::cout << error_msg << std::endl;
			continue;
		}
		std::cout << "Virheellinen syote\n";
	}

	return value;
}

