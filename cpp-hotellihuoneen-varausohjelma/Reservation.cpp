#include "Reservation.h"
#include <string>
#include "Room.h"


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

std::string Reservation::get_guest_name() const { return guest_name; }

double Reservation::get_normal_price() const { return normal_price; }

double Reservation::get_sale_percentage() const { return sale_percentage; }

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

	os << "Huone " << get_room_number() << ", " << Room::room_type_data.at(r->get_room_type()).name
		<< ". Varaaja: " << guest_name
		<< ". Öitä: " << num_nights
		<< ". Kokonaishinta: " << get_total_price() << " euroa, alennus: " << sale_percentage << "%.";
}