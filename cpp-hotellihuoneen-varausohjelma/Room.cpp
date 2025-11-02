#include "Room.h"

Room::Room(int room_number, RoomType type, bool is_occupied)
	: room_number{ room_number }, type{ type }, is_occupied{ is_occupied }
{
}

