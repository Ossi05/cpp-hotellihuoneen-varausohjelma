#pragma once
#include <string>
#include <functional>

struct MenuOption
{
	std::string name;
	std::function<void()> action;
};

