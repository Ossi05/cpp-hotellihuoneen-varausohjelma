#include "Reservation.h"
#include <string>
#include "Room.h"
#include <iomanip>
#include <sstream>
#include <vector>
#include "utils.h"
#include "exceptions.hpp"
#include "CSVReservationHandler.h"

bool operator==(const Reservation& lhs, const int id)
{
	return lhs.id == id;
}

Reservation::Reservation(
	int id,
	const std::string& guest_name,
	int room_number,
	int num_nights,
	double normal_price,
	double sale_percentage
)
	: id{ id },
	guest_name{ guest_name },
	room_number{ room_number },
	num_nights{ num_nights },
	normal_price{ normal_price },
	sale_percentage{ sale_percentage }
{
}

double Reservation::get_total_price() const {
	return normal_price * (1 - sale_percentage / 100);
}

int Reservation::get_id() const { return id; }
int Reservation::get_num_nights() const { return num_nights; }
const std::string& Reservation::get_guest_name() const { return guest_name; }
double Reservation::get_normal_price() const { return normal_price; }
double Reservation::get_sale_percentage() const { return sale_percentage; }
int Reservation::get_room_number() const { return room_number; }

RoomType Reservation::get_room_type() const {
	if (auto r{ room_ptr.lock() }) {
		return r->get_room_type();
	}
	else {
		throw RoomNotFoundException();
	}
}

void Reservation::print(std::ostream& os) const
{
	const auto r{ room_ptr.lock() };

	if (!r) {
		os << "Varauksen " << id << " huonetta ei löytynyt";
		return;
	}

	os << "ID: " << id << std::endl;
	os << "Varaaja: " << guest_name << std::endl;
	os << "Huone: " << get_room_number() << " (" << Room::ROOM_TYPE_DATA.at(r->get_room_type()).name << ")" << std::endl;
	os << "Öitä: " << num_nights << std::endl;
	os << "Normaalihinta: " << normal_price << " e"
		<< "  |  Alennus: " << sale_percentage << "%"
		<< "  |  Kokonaishinta: " << get_total_price() << " e" << std::endl;
}

/*
	Assigns a room to a reservation
*/
void Reservation::assign_room(std::shared_ptr<Room> new_room_ptr)
{
	if (new_room_ptr->is_occupied()) {
		throw RoomNotAvailableException{ "Huone " + std::to_string(new_room_ptr->get_room_number()) + " ei ole vapaana." };
	}
	if (auto r{ room_ptr.lock() }) {
		r->set_is_occupied(false);
	}

	room_ptr = new_room_ptr;
	room_number = new_room_ptr->get_room_number();
	new_room_ptr->set_is_occupied(true);
}

void Reservation::unassign_room() // Removes room reservation
{
	if (auto r{ room_ptr.lock() }) {
		r->set_is_occupied(false);
	}
	else {
		throw RoomNotFoundException();
	}
	room_ptr.reset();
}

/*
	CSV helper methods
*/
/*
	Creates a Reservation object from a CSV line
*/
Reservation Reservation::from_csv(const std::string& csv_line)
{
	std::istringstream iss{ csv_line };
	std::vector<std::string> items{};
	std::string item{};

	while (std::getline(iss, item, CSVReservationHandler::CSV_SEPARATOR)) {
		items.push_back(item);
	}

	if (items.size() != 6) {
		throw InvalidCSVFormatException{ "Virheellinen CSV-rivi: " + csv_line };
	}

	int id{ std::stoi(items.at(0)) };
	std::string guest_name{ items.at(1) };
	int room_id{ std::stoi(items.at(2)) };
	int num_nights{ std::stoi(items.at(3)) };
	double normal_price{ std::stod(items.at(4)) };
	double sale_percentage{ std::stod(items.at(5)) };

	return Reservation{
		id,
		guest_name,
		room_id,
		num_nights,
		normal_price,
		sale_percentage
	};
}

/*
	Converts reservation to a CSV line
*/
std::string Reservation::to_csv() const
{
	if (auto r{ room_ptr.lock() }) {
		std::ostringstream oss{};
		oss << id << CSVReservationHandler::CSV_SEPARATOR
			<< guest_name << CSVReservationHandler::CSV_SEPARATOR
			<< room_number << CSVReservationHandler::CSV_SEPARATOR
			<< num_nights << CSVReservationHandler::CSV_SEPARATOR
			<< normal_price << CSVReservationHandler::CSV_SEPARATOR
			<< sale_percentage;
		return oss.str();
	}
	else {
		throw RoomNotFoundException();
	}
}
