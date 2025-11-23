#include "Reservation.h"
#include <string>
#include "Room.h"
#include <iomanip>
#include <sstream>
#include <vector>
#include "utils.h"

bool operator==(const Reservation& lhs, const int id)
{
	return lhs.id == id;
}

Reservation::Reservation(
	int id,
	const std::string& guest_name,
	std::weak_ptr<Room> room,
	int num_nights,
	double normal_price,
	double sale_percentage
)
	: id{ id },
	guest_name{ guest_name },
	room{ room },
	num_nights{ num_nights },
	normal_price{ normal_price },
	sale_percentage{ sale_percentage }
{
	if (auto r{ room.lock() }) {
		r->set_is_occupied(true);
	}
}

double Reservation::get_total_price() const
{
	return normal_price * (1 - sale_percentage / 100);
}

int Reservation::get_id() const { return id; }

int Reservation::get_num_nights() const
{
	return num_nights;
}

std::string Reservation::get_guest_name() const { return guest_name; }

double Reservation::get_normal_price() const { return normal_price; }

double Reservation::get_sale_percentage() const { return sale_percentage; }

RoomType Reservation::get_room_type() const {
	if (auto r{ room.lock() }) {
		return r->get_room_type();
	}

	// TODO error handling
}


int Reservation::get_room_number() const {
	if (auto r{ room.lock() }) {
		return r->get_room_number();
	}
	else {
		// Throw error
	}
}

void Reservation::print(std::ostream& os) const
{
	const auto r{ room.lock() };

	if (!r) {
		os << "Varauksen " << id << " huonetta ei löytynyt";
		return;
	}

	os << "ID: " << id << std::endl;
	os << "Varaaja: " << guest_name << std::endl;
	os << "Huone: " << get_room_number() << " (" << Room::room_type_data.at(r->get_room_type()).name << ")" << std::endl;
	os << "Öitä: " << num_nights << std::endl;
	os << "Normaalihinta: " << normal_price << " e"
		<< "  |  Alennus: " << sale_percentage << "%"
		<< "  |  Kokonaishinta: " << get_total_price() << " e" << std::endl;
}

std::string Reservation::to_csv() const
{
	if (auto r{ room.lock() }) {
		std::ostringstream oss{};
		oss << id << CSV_SEPERATOR
			<< guest_name << CSV_SEPERATOR
			<< r->get_room_number() << CSV_SEPERATOR
			<< num_nights << CSV_SEPERATOR
			<< normal_price << CSV_SEPERATOR
			<< sale_percentage;
		return oss.str();
	}
	else {
		throw std::runtime_error("Varauksen huonetta ei löytynyt");
	}
}
