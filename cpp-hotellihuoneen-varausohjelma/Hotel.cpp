#include "Hotel.h"
#include "Room.h"
#include <iomanip>
#include <algorithm>

std::vector<Room> Hotel::generate_rooms(int num_rooms)
{
	std::vector<Room> rooms{};

	for (int i = 0; i < num_rooms; i++)
	{
		const Room room{ i, i % 2 == 0 ? RoomType::Single : RoomType::Double };
		rooms.push_back(room);
	}

	return rooms;
}

Hotel::Hotel(const std::string& name, int rooms_to_generate) : name{ name }, rooms{ generate_rooms(rooms_to_generate) }
{
}

int Hotel::get_rooms_available_amt() const
{
	return std::count_if(rooms.begin(), rooms.end(), [](const Room& room) {return !room.get_is_occupied(); });
}

// Tulostus
void Hotel::print(std::ostream& os) const
{
	const std::streamsize width{ 35 };
	const std::streamsize text_spacing_left{ 2 };
	const std::streamsize border_width{ 2 };

	const auto print_line = [&os, width]() {
		os << std::setfill('-') << std::setw(width) << "" << std::endl;
		os << std::setfill(' ');
		};
	const auto print_text = [&os, width, text_spacing_left](const std::string& text) {
		os << "|"
			<< std::setw(text_spacing_left) << " "
			<< std::left << std::setw(width - text_spacing_left - border_width) << text
			<< "|" << '\n';
		os << std::right;
		};

	print_text(name);
	print_line();
	print_text("Huoneita: " + std::to_string(rooms.size()));

	const int rooms_available = get_rooms_available_amt();
	print_text("Vapaana: " + std::to_string(rooms_available));
	print_text("Varattu: " + std::to_string(rooms.size() - rooms_available));

	print_line();
}
