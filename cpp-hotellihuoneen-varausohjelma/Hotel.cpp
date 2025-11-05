#include "Hotel.h"
#include "Room.h"
#include <iomanip>
#include <algorithm>


const int Hotel::print_width{ 35 };

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

void Hotel::print_menu_options() const
{
	std::cout << "1. Tee uusi varaus\n";
	std::cout << "2. Näytä kaikki varaukset\n";
	std::cout << "3. Hae varausta varausnumerolla\n";
	std::cout << "4. Hae varausta varaajan nimellä\n";
	std::cout << "5. Poista varaus\n";
	std::cout << "6. Hae huoneen tiedot\n";
	std::cout << "7. Näytä vapaat huoneet\n";
	std::cout << "8. Lopeta ohjelma\n";
	std::cout << std::setfill('-') << std::setw(Hotel::print_width) << "" << std::setfill(' ') << std::endl;
}

// Tulostus
void Hotel::print(std::ostream& os) const
{
	const std::streamsize text_spacing_left{ 2 };
	const std::streamsize border_width{ 2 };

	const auto print_line = [&os]() {
		os << std::setfill('=') << std::setw(Hotel::print_width) << "" << std::endl;
		os << std::setfill(' ');
		};
	const auto print_text = [&os, text_spacing_left](const std::string& text) {
		os << "|"
			<< std::setw(text_spacing_left) << " "
			<< std::left << std::setw(Hotel::print_width - text_spacing_left - border_width) << text
			<< "|" << '\n';
		os << std::right;
		};

	print_text(name);
	print_line();
	print_text("Huoneita 1-" + std::to_string(rooms.size()));
	const int rooms_available = get_rooms_available_amt();
	print_text("Vapaana: " + std::to_string(rooms_available));
	print_text("Varattu: " + std::to_string(rooms.size() - rooms_available));

	print_line();
	std::cout << std::endl;

}
