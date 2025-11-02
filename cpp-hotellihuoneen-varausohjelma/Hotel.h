#pragma once

#include "Room.h"
#include <iostream>
#include <string>
#include <vector>


class Hotel
{
private:
	std::string name;
	std::vector<Room> rooms;
	const int max_rooms;
public:
	Hotel(const std::string& name, int max_rooms);

};

