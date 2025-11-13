#include "Room.h"
#include <unordered_map>
#include <iostream>


std::unordered_map<RoomType, RoomTypeData> Room::room_type_data = {
	{ RoomType::Single, {"Yhden hengen", 100.0} },
	{ RoomType::Double, {"Kahden hengen", 200.0} }
};


Room::Room(int room_number, RoomType type, bool is_occupied)
	: room_number{ room_number }, type{ type }, _is_occupied{ is_occupied }, price{ Room::get_price(type) }
{
}

bool operator==(const Room& lhs, const int room_number)
{
	return lhs.room_number == room_number;
}

double Room::get_price() const { return this->price; }
bool Room::is_occupied() const { return this->_is_occupied; }
void Room::set_is_occupied(bool is_occupied) { this->_is_occupied = is_occupied; }
int Room::get_room_number() const { return this->room_number; }

RoomType Room::get_room_type() const { return this->type; }
double Room::get_price(RoomType type) { return Room::room_type_data.at(type).price; }
