#include "Room.h"

std::unordered_map<RoomType, double> Room::prices = {
	{ RoomType::Single, 100.0 },
	{ RoomType::Double, 200.0 },
};

Room::Room(int room_number, RoomType type, bool is_occupied)
	: room_number{ room_number }, type{ type }, is_occupied{ is_occupied }, price{Room::get_price(type)}
{}

double Room::get_price() const{ return this->price; }
bool Room::get_is_occupied() const { return this->is_occupied; }
void Room::set_is_occupied(bool is_occupied){ this->is_occupied = is_occupied; }
int Room::get_room_number() const {	return this->room_number; }

RoomType Room::get_room_type() const { return this->type; }
double Room::get_price(RoomType type) { return prices.at(type); }


