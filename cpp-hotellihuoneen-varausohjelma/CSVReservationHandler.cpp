#include "CSVReservationHandler.h"
#include "utils.h"
#include "exceptions.hpp"

CSVReservationHandler::CSVReservationHandler(const std::string& file_name) : file_name{ file_name }
{
}

void CSVReservationHandler::save_reservation(std::shared_ptr<const Reservation> reservation) const
{
	if (file_name.empty()) throw FileAccessException{ "CSV-tiedoston nimeä ei ole määritetty" };
	save_to_csv(reservation->to_csv(), file_name);
}

void CSVReservationHandler::save_all(const std::vector<std::shared_ptr<const Reservation>>& reservations) const
{
	if (file_name.empty()) throw FileAccessException{ "CSV-tiedoston nimeä ei ole määritetty" };
	clear();
	for (const auto res : reservations) {
		save_reservation(res);
	}
}

void CSVReservationHandler::remove_reservation(int id) const
{
	if (file_name.empty()) throw FileAccessException{ "CSV-tiedoston nimeä ei ole määritetty" };
	auto data{ get_data_from_csv(file_name) };

	// Poistetaan kaikki rivit, joiden id vastaa annettua id:tä
	data.erase(
		std::remove_if(data.begin(), data.end(),
			[id](const std::string& line) {
				std::istringstream iss(line);
				int line_id;
				if (!(iss >> line_id)) return false; // Virheellinen rivi, ei poisteta
				return line_id == id;
			}),
		data.end()
	);

	// Tallennetaan tiedot takaisin tiedostoon
	clear();
	for (const auto& line : data) {
		save_to_csv(line, file_name);
	}
}

void CSVReservationHandler::clear() const
{
	clear_file(file_name);
}

std::vector<std::shared_ptr<Reservation>> CSVReservationHandler::load_reservations() const
{
	std::vector<std::shared_ptr<Reservation>> reservations;

	if (file_name.empty()) throw FileAccessException{ "CSV-tiedoston nimeä ei ole määritetty" };
	try {
		auto data{ get_data_from_csv(file_name) };
		for (const auto& line : data) {
			reservations.push_back(
				std::make_shared<Reservation>(Reservation::from_csv(line))
			);
		}
		return reservations;
	}
	catch (const FileAccessException&) {
		// Tiedostoa ei löytynyt
		throw;
	}
	catch (const InvalidCSVFormatException& e) {
		// Virheellinen CSV-muoto
		throw;
	}
}

std::string CSVReservationHandler::get_file_name() const
{
	return file_name;
}
